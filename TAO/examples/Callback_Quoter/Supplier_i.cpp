// $Id$
// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Supplier_i.cpp
//
// = DESCRIPTION
//    Implementation of the Supplier class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"
#include "Supplier_i.h"
#include "ace/Reactor.h"

// Constructor.

Supplier::Supplier (void)
  : ior_ (0),
    use_naming_service_ (1),
    notifier_ (),
    loop_count_ (10)
{
}

Supplier::~Supplier (void)
{
  ACE_OS::free (this->ior_);
  ACE_DEBUG ((LM_DEBUG,
              "Market Status Supplier daemon exiting!\n"));

  
}

// Reads the Server factory IOR from a file.

int
Supplier::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                        filename,"file_open"),
                        -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n",
                       "read_file"),
                      -1);

  this->ior_ = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

int
Supplier::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:xk:xs");

  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++; //****
        break;

      case 'n':  // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;

      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.optarg);
        break;

      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg,
                             "get_args"),
                            -1);
        break;

      case 's': // don't use the naming service
        this->use_naming_service_ = 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f ior-file]"
                           " [-k ior]"
                           " [-x]"
                           " [-s]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Give the stock status information to the Notifier.

int
Supplier::send_market_status (const char *stock_name,
                                       long value)
{
  TAO_TRY
    {
      // Make the RMI.
      this->notifier_->market_status (stock_name,
                                    value,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Exception raised!\n"),
                        -1);
    }
  TAO_ENDTRY;
  return 0;
}

// Execute client example code.

int
Supplier::run (void)
{
  
  long timer_id = 0;
 
  ACE_DEBUG ((LM_DEBUG,
              "Market Status Supplier Daemon is running...\n "));

  
  ACE_Time_Value period (1);

  
  timer_id = reactor_used ()->schedule_timer (supplier_timer_handler_, 
                                                  "Periodic stockfeed",
                                                  period,
                                                 period);
   if ( timer_id== -1)
        ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "schedule_timer"), -1);
                                            
   // ACE_DEBUG ((LM_DEBUG,
   //               "cancelling timer\n"));
   //  this->reactor_used ()->cancel_timer (timer_id);

  /*

  // @@ Ultimately, let's replace this with an ACE_OS::sleep() call or
  // something.
  for (;;)
    {
      long value;
      char stock_name[BUFSIZ];

      // Obtain input from the user.
      scanf ("%s %ld", stock_name, &value);

      // Make a call to the method which will send the current market
      // status.
      if (this->send_market_status (stock_name, value) < 0)
        break;
    }
  */
   this->reactor_used ()->run_event_loop ();
  return 0;
}


int
Supplier::via_naming_service (void)
{
  TAO_TRY
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);
      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id = CORBA::string_dup ("Notifier");

      CORBA::Object_var notifier_obj =
        this->naming_services_client_->resolve (notifier_ref_name,
                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to Notifier_var using
      // the <_narrow> method.
      this->notifier_ =
        Notifier::_narrow (notifier_obj.in (),
                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Supplier::via_naming_service\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Init function.

int
Supplier::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_TRY
    {

      // Create the Timer_Handler.
      ACE_NEW_RETURN (supplier_timer_handler_,
                  Supplier_Timer_Handler (this,this->reactor_used ()),
                  -1);

     
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    0,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->use_naming_service_)
        return via_naming_service ();

      if (this->ior_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no ior specified\n",
                           this->argv_[0]),
                          -1);

      CORBA::Object_var notifier_object =
        this->orb_->string_to_object (this->ior_,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (notifier_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                          -1);

      // The downcasting from CORBA::Object_var to Notifier_var is done
      // using the <_narrow> method.
      this->notifier_ = Notifier::_narrow (notifier_object.in (),
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Supplier::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


ACE_Reactor*
Supplier::reactor_used (void) const
{
  return (ACE_Reactor::instance ());
}

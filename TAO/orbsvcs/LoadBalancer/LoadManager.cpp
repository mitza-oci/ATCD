#include "orbsvcs/LoadBalancing/LB_LoadManager.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"


ACE_RCSID (LoadBalancer,
           LoadBalancer,
           "$Id$")


static const char * lb_ior_file = "lb.ior";

void
usage (const ACE_TCHAR * cmd)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:  %s ")
              ACE_TEXT ("-o <ior_output_file> ")
              ACE_TEXT ("-s <RoundRobin|Random|LeastLoaded> ")
              ACE_TEXT ("-h ")
              ACE_TEXT ("\n\n")
              ACE_TEXT (" NOTE: Standard default values will be used ")
              ACE_TEXT ("for \"LeastLoaded\" strategy.\n"),
              cmd));
}

void
parse_args (int argc,
            ACE_TCHAR *argv[],
            int & default_strategy
            ACE_ENV_ARG_DECL)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:s:h"));

  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ::lb_ior_file = get_opts.opt_arg ();
          break;

        case 's':
          if (ACE_OS::strcasecmp (get_opts.opt_arg (),
                                  "RoundRobin") == 0)
            default_strategy = 0;
          else if (ACE_OS::strcasecmp (get_opts.opt_arg (),
                                       "Random") == 0)
            default_strategy = 1;
          else if (ACE_OS::strcasecmp (get_opts.opt_arg (),
                                       "LeastLoaded") == 0)
            default_strategy = 2;
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Unknown strategy, using RoundRobin\n")));
          break;

        case 'h':
          ::usage (argv[0]);
          break;

        default:
          ::usage (argv[0]);
          ACE_THROW (CORBA::BAD_PARAM ());
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // The usual server side boilerplate code.

      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // "built-in" strategies are the following:
      //   0 = RoundRobin
      //   1 = Random
      //   2 = LeastLoaded
      int default_strategy = 0;

      // Check the non-ORB arguments.
      ::parse_args (argc,
                    argv,
                    default_strategy
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_LB_LoadManager * lm;
      ACE_NEW_THROW_EX (lm,
                        TAO_LB_LoadManager,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableServer::ServantBase_var safe_lm = lm;

      // Initalize the LoadManager servant.
      lm->init (orb->orb_core ()->reactor (),
                root_poa.in ()
                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadManager_var load_manager =
        lm->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::Properties props (1);
      props.length (1);
      props[0].nam.length (1);
      props[0].nam[0].id =
        CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy");

      switch (default_strategy)
        {
        case 0:
          props[0].val <<= "RoundRobin";
          break;
        case 1:
          props[0].val <<= "Random";
          break;
        case 2:
          props[0].val <<= "LeastLoaded";
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("ERROR: LoadBalancer internal error.\n")
                            ACE_TEXT ("       Unknown built-in strategy.\n")),
                            -1);
        }

      load_manager->set_default_properties (props
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb->object_to_string (load_manager.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      FILE * lb_ior = ACE_OS::fopen (lb_ior_file, "w");
      ACE_OS::fprintf (lb_ior, "%s", str.in ());
      ACE_OS::fclose (lb_ior);

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO Load Balancer");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

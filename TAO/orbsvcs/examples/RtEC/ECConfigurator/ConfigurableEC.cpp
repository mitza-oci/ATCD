// $Id$

#include "ace/Get_Opt.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/DefaultHandler.h"

#include "Configurator_ParseHandler.h"

struct Arguments
{
  ACE_CString filename_;
};

int parse_args (int argc, char *argv[],Arguments &args);

int
main (int argc, char *argv[])
{
  int retval;

  ACEXML_TRY_NEW_ENV
    {
      ACEXML_Parser parser;
      Arguments args;
      args.filename_.set(ACE_TEXT("test.xml"));

      // parse args for config filename
      if (parse_args(argc,argv,args) == -1)
        {
          return 1;
        }

      ACEXML_FileCharStream *fcs = new ACEXML_FileCharStream();
      if ((retval = fcs->open(args.filename_.c_str())) != 0) {
        //ACE_DEBUG ((LM_DEBUG, "Could not open file %s\n",args.filename_.c_str()));
        return retval;
      }

      ACEXML_InputSource is (fcs); //takes responsibility of fcs

      Configurator_ParseHandler xmlhandler (args.filename_.c_str());
      ACEXML_DefaultHandler dflt;

      parser.setContentHandler (&xmlhandler);
      parser.setDTDHandler (&dflt);
      parser.setErrorHandler (&xmlhandler);
      parser.setEntityResolver (&dflt);

      parser.parse(&is);
      ACEXML_TRY_CHECK;

      if ((retval = fcs->close()) != 0) {
        //ACE_DEBUG ((LM_DEBUG, "Could not close file %s\n",args.filename_.c_str()));
        return retval;
      }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished parsing\n")));

    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred: %s. Exiting...\n"),
                  ex.message()));
      return 1;
    }
  ACEXML_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished successfully\n")));

  return retval;
}

int parse_args (int argc, char *argv[], Arguments &args)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        args.filename_.set(get_opts.opt_arg());
        //ACE_DEBUG((LM_DEBUG,ACE_TEXT("Filename argument: %s\n"),args.filename_.c_str()));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f <filename>] "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

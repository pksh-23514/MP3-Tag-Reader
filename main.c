#include <stdio.h>
#include "types.h"
#include "view.h"
#include "edit.h"

int main(int argc, char* argv[])
{
    TagInfo mp3tagInfo;
    TagData mp3tagData;

    if (argc < 2)		//If the argument count passed in Command Line are improper, print the format in which the arguments shall be passed.
    {
        printf ("ERROR: Incorrect format of Command Line Arguments.\n");
        printf ("INFO: Use \"./mp3_tag_reader --help\" for Help menu.\n");
    }
    else
    {
        OperationType ret = check_operation (argv);		//To check which Operation needs to be performed based on the Option passed in Command Line.

        if (ret == p_view)				//If the return value from the function is to perform Viewing the MP3 Tag, proceed for Validation and Viewing process.
        {
            printf ("--------------------------\n");
            printf ("MP3 Tag Reader and Editor\n");
            printf ("--------------------------\n");
            Status ret1 = read_and_validate_mp3_file (argv, &mp3tagInfo);		//To validate all the arguments passed in Command Line.
            if (ret1 == p_success)			//If validation is successful, proceed with the Viewing process.
            {
                Status ret2 = view_tag (argv, &mp3tagInfo);						//To execute the Viewing process of the MP3 Tag.
                if (ret2 == p_success)		//If the Viewing process is successfully executed, print a confirmation message.
                {
                    printf ("INFO: Done.\n");
                    printf ("--------------------------\n");
                }
            }
        }
        else if (ret == p_edit)			//If the return value from the function is to perform Editing the MP3 Tag, proceed for Validation and Editing process.
        {
            printf ("MP3 Tag Reader and Editor:\n");
            printf ("--------------------------\n");
            Status ret1 = read_and_validate_mp3_file_args (argv, &mp3tagData);	//To validate all the arguments passed in Command Line.
            if (ret1 == p_success)			//If validation is successful, proceed with the Editing process.
            {
                Status ret2 = edit_tag (argv, &mp3tagData);						//To execute the Editing process of the MP3 Tag.
                if (ret2 == p_success)		//If the Editing process is successfully executed, print a confirmation message.
                {
                    printf ("INFO: Done.\n");
                    printf ("--------------------------\n");
                    //Call the view_tag function to display the Updated content.
                }
            }
        }
        else if (ret == p_help)			//If the return value from the function is to show the Help Menu, display all the details regarding the usage of MP3 Tag Reader & Editor.
        {
            printf ("INFO: Help Menu for Tag Reader and Editor:\n");
            printf ("INFO: For Viewing the Tags -> ./mp3_tag_reader -v <file_name.mp3>\n");
            printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
            printf ("INFO: Modifier Functions:\n");
            printf ("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
        }
        else if (ret == p_unsupported)	//If the return value from the function is other than to perform View/Edit/Help, print an error message.
        {
            printf ("ERROR: Unsupported Operation.\n");
            printf ("INFO: Use \"./mp3_tag_reader --help\" for Help menu.\n");
        }
    }

    return 0;
}

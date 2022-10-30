#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "view.h"

/* Function Definitions */

/* Check which Operation is to be Performed
 * Input: Command Line Arguments
 * Output: Operation to be Performed
 * Return Value: p_view, p_edit, p_help or p_unsupported on Invalid options provided by User
 */

OperationType check_operation (char* argv[])
{
    if (strncmp (argv [1], "-v", 2) == 0)
    {
        return p_view;
    }
    else if (strncmp (argv [1], "-e", 2) == 0)
    {
        return p_edit;
    }
    else if (strncmp (argv [1], "--help", 6) == 0)
    {
        return p_help;
    }
    else
    {
        return p_unsupported;
    }
}

/* Check if the Read Command Line Arguments are Valid
 * Input: Command Line Arguments and Structure Pointer
 * Output: Source MP3 file
 * Return Value: p_success or p_failure on Invalid Arguments passed
 */

Status read_and_validate_mp3_file (char* argv[], TagInfo* mp3tagInfo)
{
    if (argv [2] == NULL)		//The Source MP3 file name shall be passed as the 2nd Command Line Argument as per the format.
    {
        printf ("INFO: For Viewing the Tags -> ./mp3_tag_reader -v <file_name.mp3>\n");
        return p_failure;
    }
    else
    {
        mp3tagInfo->fptr_mp3 = fopen (argv [2], "r");		//Open the file in Read Only Mode.
        if (mp3tagInfo->fptr_mp3 != NULL)
        {
            char str[3];
            fread (str, 1, 3, mp3tagInfo->fptr_mp3);		//Read the file Signature. For our MP3 file, it is "ID3".
            if ((strncmp (str, "ID3", 3)) == 0)				//Check if the value read in 'str' is matching with the MP3 file Signature "ID3".
            {
                printf ("     Version ID: v2.3     \n");
                printf ("--------------------------\n");
            }
            else		//If the file Signature is not "ID3", it is not a '.mp3' file and no further operation can be performed.
            {
                printf ("ERROR: The file Signature is not matching with that of a '.mp3' file.\n");
                printf ("INFO: For Viewing the Tags -> ./mp3_tag_reader -v <file_name.mp3>\n");
                return p_failure;
            }
        }
        else		//If the file does not open, we cannot read and validate the arguments to the Structure. So, return 'p_failure'.
        {
            printf ("ERROR: Unable to Open the %s file.\n", argv [2]);
            printf ("INFO: For Viewing the Tags -> ./mp3_tag_reader -v <file_name.mp3>\n");
            return p_failure;
        }

        fclose (mp3tagInfo->fptr_mp3);						//Close the file opened previously.
    }

    //No p-failure returned above, then return p_success.
    return p_success;
}

/* To view all the Frame contents of the MP3 file Tag
 * Input: Command Line Arguments and Structure Pointer
 * Output: All the Frame Contents
 * Return Value: p_success or p_failure
 */

Status view_tag (char* argv[], TagInfo* mp3tagInfo)
{
    mp3tagInfo->fptr_mp3 = fopen (argv [2], "r");		//Open the file in Read Only Mode.
    fseek (mp3tagInfo->fptr_mp3, 10L, SEEK_SET);		//The first 10 bytes of the '.mp3' file is ID3 Header and need to be skipped.
	Status ret;

	//Function call with the necessary parameters to display the Information about Title tag.
    ret = get_and_display_data ("Title: ", "TIT2", mp3tagInfo->frame_Id, &mp3tagInfo->title_tag_size, mp3tagInfo->title_tag, mp3tagInfo->fptr_mp3);
    if (ret == p_failure)		//If the function doesn't display the data successfully, the process of Displaying will terminate.
    {
        printf ("ERROR: Title Frame ID Contents cannot be displayed.\n");
        return p_failure;
    }

	//Function call with the necessary parameters to display the Information about Artist tag.
    ret = get_and_display_data ("Artist: ", "TPE1", mp3tagInfo->frame_Id, &mp3tagInfo->artist_tag_size, mp3tagInfo->artist_tag, mp3tagInfo->fptr_mp3);
    if (ret == p_failure)		//If the function doesn't display the data successfully, the process of Displaying will terminate.
    {
        printf ("ERROR: Artist Frame ID Contents cannot be displayed.\n");
        return p_failure;
    }

	//Function call with the necessary parameters to display the Information about Album tag.
    ret = get_and_display_data ("Album: ", "TALB", mp3tagInfo->frame_Id, &mp3tagInfo->album_tag_size, mp3tagInfo->album_tag, mp3tagInfo->fptr_mp3);
    if (ret == p_failure)		//If the function doesn't display the data successfully, the process of Displaying will terminate.
    {
        printf ("ERROR: Album Frame ID Contents cannot be displayed.\n");
        return p_failure;
    }

	//Function call with the necessary parameters to display the Information about Year tag.
    ret = get_and_display_data ("Year: ", "TYER", mp3tagInfo->frame_Id, &mp3tagInfo->year_size, mp3tagInfo->year, mp3tagInfo->fptr_mp3);
    if (ret == p_failure)		//If the function doesn't display the data successfully, the process of Displaying will terminate.
    {
        printf ("ERROR: Year Frame ID Contents cannot be displayed.\n");
        return p_failure;
    }

	//Function call with the necessary parameters to display the Information about Content tag.
    ret = get_and_display_data ("Content: ", "TCON", mp3tagInfo->frame_Id, &mp3tagInfo->content_type_size, mp3tagInfo->content_type, mp3tagInfo->fptr_mp3);
    if (ret == p_failure)		//If the function doesn't display the data successfully, the process of Displaying will terminate.
    {
        printf ("ERROR: Content Type Frame ID Contents cannot be displayed.\n");
        return p_failure;
    }

	//Function call with the necessary parameters to display the Information about Comments tag.
    ret = get_and_display_data ("Comments: ", "COMM", mp3tagInfo->frame_Id, &mp3tagInfo->comment_size, mp3tagInfo->comments, mp3tagInfo->fptr_mp3);
    if (ret == p_failure)		//If the function doesn't display the data successfully, the process of Displaying will terminate.
    {
        printf ("ERROR: Content Type Frame ID Contents cannot be displayed.\n");
        return p_failure;
    }

    printf ("--------------------------\n");
    fclose (mp3tagInfo->fptr_mp3);				//To close the Source MP3 file.

    //No p-failure returned above, then return p_success.
    return p_success;
}

/* To view the contents of a particular Frame ID
 * Input: Frame ID, Frame ID Tag (desired), Frame ID tag (to be extracted from '.mp3' file, Frame ID size, Frame ID contents and File pointer
 * Output: Frame ID contents are displayed
 * Return Value: p_success or p_failure
 */

Status get_and_display_data (const char* str_frame, const char* str_Id, char* frame_Id, uint* tag_size, char* tag, FILE* fptr)
{
    int count;
	printf ("MP3 position = %ld.\n", ftell (fptr));
    count = fread (frame_Id, 1, 4, fptr);				//To read the 4 characters of the Source MP3 file for the Frame ID tag present.
    if (count < 4)										//Error Handling.
    {
        printf ("ERROR: Unable to read from the MP3 file.\n");
        return p_failure;
    }
    else
    {
        if ((strncmp (frame_Id, str_Id, 4)) == 0)		//To check if the Frame ID extracted matches with the desired Frame ID whose contents need to be displayed.
        {
            count = fread (tag_size, 4, 1, fptr);		//To read the 4 characters of the Source MP3 file for the size of the Frame ID content.
            
            /*fread() stores the size in the reverse order of what is present in the Hex dump of the MP3 file.
              Eg.- Hex dump: 00 00 00 2f (MSB to LSB).
                   fread() stores the value in 'tag_size' as: 00 00 00 2f (LSB to MSB)
              So, the Bytes shall be reversed after storing in 'tag_size' to get the Correct value.*/

            if (count < 1)								//Error Handling.
            {
                printf ("ERROR: Unable to read from the MP3 file.\n");
                return p_failure;
            }
            else										//Reversal of the Bytes after reading to get the correct value of the 'tag_size'.
            {
                char* pos = (char*) tag_size;
                for (int i = 0; i < ((sizeof (uint)) / 2); i++)
                {
                    pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                    pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                    pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                }

                printf ("%sSize = %u.\n", str_frame, *tag_size);
                tag = (char*) malloc (((*tag_size) * sizeof (char)));		//To allocate the memory dynamically for storing the contents of the Frame ID.

                fseek (fptr, 3L, SEEK_CUR);		//1 byte of Flag is included in the size and 2 bytes of Flag are to be skipped. Total of 3 bytes of Flag shall be skipped.
                //printf ("Start: %ld.\n", ftell (fptr));

				//Suppose the Frame content size is 47, it includes 46 bytes of Frame ID content and 1 byte of Flag. So, we skip 3 bytes of Flag and read (size-1) for Frame ID content.
                count = fread (tag, 1, (*tag_size - 1), fptr);		//To read the (tag_size - 1) bytes of the Source MP3 file for the Frame ID content.
                if (count < (*tag_size - 1))						//Error Handling.
                {
                    printf ("ERROR: Unable to read from the MP3 file.\n");
                    return p_failure;
                }
                else
                {
                    tag [*tag_size - 1] = '\0';						//To end the Frame ID content with '\0' as last character.
                    //printf ("End: %ld.\n", ftell (fptr));
                    printf ("%-10s%s.\n", str_frame, tag);			//To display the Frame ID Contents.
                }
            }
        }
        else		//If the Frame ID extracted from '.mp3' file doesn't matches with the desired Frame ID, the process of Displaying shall terminate.
        {
            printf ("ERROR: %s Frame ID not found.\n", str_Id);
            return p_failure;
        }
    }

    free (tag);			//To free the memory allocated to the Frame ID content.
    tag = NULL;			//To prevent the 'tag' from becoming a dangling pointer.

    //No p-failure returned above, then return p_success.
    return p_success;
}

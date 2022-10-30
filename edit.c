#include <stdio.h>
#include <string.h>
#include "types.h"
#include "edit.h"

Status read_and_validate_mp3_file_args (char* argv[], TagData* mp3tagData)
{
    if (argv [2] == NULL)
    {
        printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        printf ("INFO: Modifier Functions:\n");
        printf ("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
        return p_failure;
    }
    else
    {
        if ((strncmp (argv [2], "-t", 2) == 0))
        {
            strncpy (mp3tagData->frame_Id, "TIT2", 5);
        }
        else if ((strncmp (argv [2], "-A", 2) == 0))
        {
            strncpy (mp3tagData->frame_Id, "TPE1", 5);
        }
        else if ((strncmp (argv [2], "-a", 2) == 0))
        {
            strncpy (mp3tagData->frame_Id, "TALB", 5);
        }
        else if ((strncmp (argv [2], "-y", 2) == 0))
        {
            strncpy (mp3tagData->frame_Id, "TYER", 5);
        }
        else if ((strncmp (argv [2], "-G", 2) == 0))
        {
            strncpy (mp3tagData->frame_Id, "TCON", 5);
        }
        else if ((strncmp (argv [2], "-c", 2) == 0))
        {
            strncpy (mp3tagData->frame_Id, "COMM", 5);
        }
        else
        {
            printf ("ERROR: Unsupported Modifier.\n");
            printf ("INFO: Modifier Functions:\n");
            printf ("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
            return p_failure;
        }
        printf ("Frame ID to be changed: %s.\n", mp3tagData->frame_Id);
    }

    if (argv [3] == NULL)
    {
        printf ("ERROR: New_Value to be updated on the Frame ID %s is Empty.\n", mp3tagData->frame_Id);
        printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        return p_failure;
    }
    else
    {
        mp3tagData->frame_Id_size = strlen (argv [3]) + 1;
        if (mp3tagData->frame_Id_size < 100)
        {
            printf ("Length of the Data in CLA = %d.\n", mp3tagData->frame_Id_size);
            strncpy (mp3tagData->frame_Id_value, argv [3], mp3tagData->frame_Id_size);
            printf ("Updated Value of Frame ID is %s.\n", mp3tagData->frame_Id_value);
        }
        else
        {
            printf ("ERROR: Length of the Data is too Long!.\n");
            return p_failure;
        }
    }

    if (argv [4] == NULL)
    {
        printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        return p_failure;
    }
    else
    {
        mp3tagData->fptr_mp3 = fopen (argv [4], "r");
        if (mp3tagData->fptr_mp3 != NULL)
        {
            char str[3];
            fread (str, 1, 3, mp3tagData->fptr_mp3);
            if ((strncmp (str, "ID3", 3)) == 0)
            {
                printf ("     Version ID: v2.3     \n");
                printf ("--------------------------\n");
            }
            else
            {
                printf ("ERROR: The file Signature is not matching with that of a '.mp3' file.\n");
                printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
                return p_failure;
            }
        }
        else
        {
            printf ("ERROR: Unable to Open the %s file.\n", argv [4]);
            printf ("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
            return p_failure;
        }

        fclose (mp3tagData->fptr_mp3);
    }

    //No p-failure returned above, then return p_success.
    return p_success;
}

Status edit_tag (char* argv[], TagData* mp3tagData)
{
    mp3tagData->fptr_mp3 = fopen (argv [4], "r");
    mp3tagData->fptr_temp = fopen ("temp.mp3", "w");

    fseek (mp3tagData->fptr_mp3, 0L, SEEK_SET);
    fseek (mp3tagData->fptr_temp, 0L, SEEK_SET);

    char str [10];
    int count;
    count  = fread (str, 1, 10, mp3tagData->fptr_mp3);
    if (count < 10)
    {
        printf ("ERROR: Unable to read from the MP3 file.\n");
        return p_failure;
    }
    else
    {
        count = fwrite (str, 1, 10, mp3tagData->fptr_temp);
        if (count < 10)
        {
            printf ("ERROR: Unable to write on the MP3 file.\n");
            return p_failure;
        }
    }

    char* frame_tag [6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    for (int i = 0; i < 6; i++)
    {
        count = fread (str, 1, 4, mp3tagData->fptr_mp3);
        if (count < 4)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            count = fwrite (str, 1, 4, mp3tagData->fptr_temp);
            if (count < 4)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return p_failure;
            }
            else
            {
                if ((strncmp (mp3tagData->frame_Id, frame_tag [i], 4)) == 0)
                {
                    Status ret = copy_data (mp3tagData, 1);
                    if (ret == p_failure)
                    {
                        printf ("ERROR: Unable to copy the updated Data from Source to Destination '.mp3' file.\n");
                        return p_failure;
                    }
                }
                else
                {
                    Status ret = copy_data (mp3tagData, 0);
                    if (ret == p_failure)
                    {
                        printf ("ERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\n");
                        return p_failure;
                    }
                }
            }
        }
    }

    Status ret;
    ret = copy_remaining_data (mp3tagData);
    if (ret == p_failure)
    {
        printf ("ERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\n");
        return p_failure;
    }
    
	fclose (mp3tagData->fptr_mp3);
	fclose (mp3tagData->fptr_temp);

	mp3tagData->fptr_mp3 = fopen (argv [4], "w");
	mp3tagData->fptr_temp = fopen ("temp.mp3", "r");
    
	ret = copy_back_to_src (mp3tagData);
    if (ret == p_failure)
    {
        printf ("ERROR: Unable to copy the remaining Data from Destination to Source '.mp3' file.\n");
        return p_failure;
    }

    //No p-failure returned above, then return p_success.
    return p_success;
}

Status copy_data (TagData* mp3tagData, char flag)
{
    if (flag == 1)
    {
        uint old_size, new_size;
        int count;
        count = fread (&old_size, 4, 1, mp3tagData->fptr_mp3);
        if (count < 1)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            char* pos = (char*) &old_size;
            for (int i = 0; i < ((sizeof (uint)) / 2); i++)     //Reversal of the Bytes after reading to get the correct value of the 'Size'.
            {
                pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
                pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            }
        }
        printf ("Old Tag Data Size = %d.\n", old_size);
        
        new_size = mp3tagData->frame_Id_size;
        char* pos = (char*) &mp3tagData->frame_Id_size;
        for (int i = 0; i < ((sizeof (uint)) / 2); i++)     //Reversal of the Bytes after reading to get the correct value of the 'Size'.
        {
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
        }
        printf ("New Tag Data Size = %X.\n", mp3tagData->frame_Id_size);

        count = fwrite (&mp3tagData->frame_Id_size, 4, 1, mp3tagData->fptr_temp);
        if (count < 1)
        {
            printf ("ERROR: Unable to write on the MP3 file.\n");
            return p_failure;
        }

        char f [3];
        count = fread (f, 1, 3, mp3tagData->fptr_mp3);
        if (count < 3)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            count = fwrite (f, 1, 3, mp3tagData->fptr_temp);
            if (count < 3)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return p_failure;
            }
        }

        count = fwrite (mp3tagData->frame_Id_value, 1, (new_size - 1), mp3tagData->fptr_temp);
        if (count < (new_size - 1))
        {
            printf ("ERROR: Unable to write on the MP3 file.\n");
            return p_failure;
        }
        fseek (mp3tagData->fptr_mp3, (long) (old_size - 1), SEEK_CUR);
        printf ("New Data = %s.\n", mp3tagData->frame_Id_value);
        printf ("MP3 position = %lu.\n", ftell (mp3tagData->fptr_mp3));
        printf ("Temp position = %lu.\n", ftell (mp3tagData->fptr_temp));
    }
    else
    {
        uint size;
        int count;
        count = fread (&size, 4, 1, mp3tagData->fptr_mp3);
        if (count < 1)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            count = fwrite (&size, 4, 1, mp3tagData->fptr_temp);
            if (count < 1)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return p_failure;
            }

        }
        char* pos = (char*) &size;
        for (int i = 0; i < ((sizeof (uint)) / 2); i++)     //Reversal of the Bytes after reading to get the correct value of the 'Size'.
        {
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [(sizeof (uint)) - i - 1] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
            pos [i] = pos [i] ^ pos [(sizeof (uint)) - i - 1];
        }
        printf ("Size of the Frame ID content = %d.\n", size);

        char f [3];
        count = fread (f, 1, 3, mp3tagData->fptr_mp3);
        if (count < 3)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            count = fwrite (f, 1, 3, mp3tagData->fptr_temp);
            if (count < 3)
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return p_failure;
            }
        }

        char str [size-1];
        count = fread (str, 1, (size - 1), mp3tagData->fptr_mp3);
        if (count < (size - 1))
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            str [size - 1] = '\0';
            printf ("Frame ID content = %s.\n", str);
            count = fwrite (str, 1, (size - 1), mp3tagData->fptr_temp);
            if (count < (size - 1))
            {
                printf ("ERROR: Unable to write on the MP3 file.\n");
                return p_failure;
            }
        }

        printf ("MP3 position = %lu.\n", ftell (mp3tagData->fptr_mp3));
        printf ("Temp position = %lu.\n", ftell (mp3tagData->fptr_temp));
    }

    //No p-failure returned above, then return p_success.
    return p_success;
}

Status copy_remaining_data (TagData* mp3tagData)
{
    printf ("Copy Remaining Data:\n");
    char ch [1024];
    int count;
    long start = ftell (mp3tagData->fptr_mp3);
    printf ("MP3 position = %lu.\n", start);
    printf ("Temp position = %lu.\n", ftell (mp3tagData->fptr_temp));

    fseek (mp3tagData->fptr_mp3, 0L, SEEK_END);
    long end = ftell (mp3tagData->fptr_mp3);
    printf ("MP3 position = %lu.\n", end);

    fseek (mp3tagData->fptr_mp3, start, SEEK_SET);
    printf ("MP3 position = %lu.\n", ftell (mp3tagData->fptr_mp3));

    while ((ftell (mp3tagData->fptr_mp3)) != end)
    {
        count = fread (ch, 1, 1024, mp3tagData->fptr_mp3);
        if (count < 0)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            fwrite (ch, 1, count, mp3tagData->fptr_temp);
        }
    }

     printf ("MP3 position = %lu.\n", ftell (mp3tagData->fptr_mp3));
     printf ("Temp position = %lu.\n", ftell (mp3tagData->fptr_temp));

     //No p-failure returned above, then return p_success.
     return p_success;
}

Status copy_back_to_src (TagData* mp3tagData)
{
	printf ("Copy Data Back to Source:\n");
	fseek (mp3tagData->fptr_mp3, 0L, SEEK_SET);
	fseek (mp3tagData->fptr_temp, 0L, SEEK_SET);
	int count;
	char ch [1024];
	long start = ftell (mp3tagData->fptr_temp);
    printf ("Temp position = %lu.\n", start);
    printf ("MP3 position = %lu.\n", ftell (mp3tagData->fptr_mp3));

    fseek (mp3tagData->fptr_temp, 0L, SEEK_END);
    long end = ftell (mp3tagData->fptr_temp);
    printf ("Temp position = %lu.\n", end);

    fseek (mp3tagData->fptr_temp, 0L, SEEK_SET);
    printf ("Temp position = %lu.\n", ftell (mp3tagData->fptr_temp));

    while ((ftell (mp3tagData->fptr_temp)) != end)
    {
        count = fread (ch, 1, 1024, mp3tagData->fptr_temp);
        if (count < 0)
        {
            printf ("ERROR: Unable to read from the MP3 file.\n");
            return p_failure;
        }
        else
        {
            fwrite (ch, 1, count, mp3tagData->fptr_mp3);
        }
    }

     printf ("MP3 position = %lu.\n", ftell (mp3tagData->fptr_mp3));
     printf ("Temp position = %lu.\n", ftell (mp3tagData->fptr_temp));

	 //Update the new File size in the Header of sample.mp3.

     //No p-failure returned above, then return p_success.
     return p_success;
}

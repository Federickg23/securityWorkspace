# Security HW 4

To run this project, please run `create-users.sh` with `sudo bash create-users.sh`. This will create all the groups and users necessary for the code to work properly, including all the users included in Professor Bellovin's initial mailbox_gen.sh file, and one additional mail user, and the same is true for groups.  

Once this has been done, simply input the following command: `make DEST=destination`, where destination is the name of the email directory you wish to create. If the directory already exists, the makefile will exit with an error code of 1, so please make sure to remove the created directory before running make a second time. If you wish to use the mail-in executable, cd into the top level of the directory you created and run `./bin/mail-in < testfile`, where the testfile is a message formatted according to the specs of homework 3. 

When creating the directory, each mailbox is given to its own user, and each user has read, write, and execute permissions on their own mailbox. Each mailbox is also a member of the group known as mail, but the users are not members of this group. These mailboxes also have group permissions of read write and execute, so that they can be accessed by a privileged mail-out program. Users should not be able to access eachother's mailboxes. 

mail-in and mail-out are both owned by the user known as mail. mail-in has its uid bit set, and mail-out has its gid bit set, so that when running mail-in it is run as the mail user, and when running mail-out it is run as the mail group. If one is not the mail user, it cannot access mail-out. 

Finally, the bin and mail folder only have read and excecute permissions for all, allowing users to see the contents and run files within them, but not allowing them to delete directories, modify or delete files. 
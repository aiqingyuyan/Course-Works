#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <stdexcept>
#include <ext/stdio_filebuf.h>

//---------------------------------------------------------------//
//              * *    DO NOT MODIFY THIS FILE   *  *            //
//---------------------------------------------------------------//

/**
   A convenience class to convert HTML (from a given URL) to standard
   plain text.
   
   This class internally creates a process and provide a simple
   wrapper around its artifacts for obtaining text content from an
   online HTML resource.
   
   NOTE: Currently, this class can properly used only from MPI process
   with Rank 0.  On other ranks the behavior of this class is
   unpredictable.
*/
class HTMLDownloader {
public:
    /**
       Constructor to create a HTMLDownloader object to download data
       from a given URL.

       \param[in] url The fully qualified URL from where HTML is to be
       downloaded and converted to text by this class.
    */
    HTMLDownloader(const std::string& url);

    /** The destructor.

        The destructor winds-down any pending streams and internal
        data managed by this class.
    */
    ~HTMLDownloader();
    
    /**
       Obtain a stream from where the text of the HTML content from
       the supplied URL can be obtained.  The HTML content is read
       from the URL that was specified when this object was
       instantiated.
    */
    std::istream& getText() throw (std::exception&);
    
private:
    /** Primary method to create a child-process.

		This method forks the current process to create a child
        process.  If the fork is successful, then this method returns
        the pid (from fork) to distinguish between parent and child.
        This method also redirects (using pipes) the standard output
        streams via call to the getText() method.

        \return On successful forking this method returns 0 in the
        child and a non-zero value in the parent.

        \exception std::runtime_error On errors, a child process is
        not created and this method throws std::runtime_error
        exception.
    */
    int create() throw (std::runtime_error&);

    /** Convenience method to run a given command.

		This method converts the given command-line to a vector of
        strings (using white space as argument delimiter) and uses the
        overloaded create() methods to run the command.

        NOTE: This method cannot be used to pass blank-spaces as
        arguments to the child process.
		
        \param[in] cmdLine The name of the command along with
        arguments.  Example of strings that can be passed-in to this
        method are: "ps", "find /usr -name stdio_*.h -print", "blah
        blah-be-de-blah blah-blah", etc.) to be run.  Assuming the
        following about the string:
        
        - Words in the string are delimited by 1 or more white-spaces.
        
        - The first word indicates the command to be executed and
          rest of the words are command-line arguments to be passed
          to the program to be run.

        \return On successful forking this method returns 0 in the
        child and a non-zero value in the parent.
        
        \exception std::runtime_error This method throws exceptions on
        errors.
    */
    int create(const std::string& cmdLine) throw (std::runtime_error&);

    /** Convenience method to run a given command as a child-process.

        \param[in] args The command and command-line arguments for
        the program to be executed as a separate child process.  The
        first entry in this vector is assumed to be the command to
        be executed by this method.
        
        \exception std::runtime_error This method throws exceptions
        on errors.
    */
    int create(const std::vector<std::string>& args)
        throw (std::runtime_error&);

    /**
       The URL from where the HTML is to be obtained and converted to
       text by this class.
    */
    std::string url;
    
    /**
       This instance variable that tracks the pipe handles that is
       used to read standard output from a child process.
    */
    int pipeHandle;
	
    /**
       The input stream from where the standard output for this
       process can be read.  This stream wraps the end of the pipe
       that is tied to the output stream of the child process.
    */
    std::istream* stdout;

    /**
       The file buffer that is used to wrap the native OS pipe
       handles.
    */
    __gnu_cxx::stdio_filebuf<char> *pipeFileBuf;
};

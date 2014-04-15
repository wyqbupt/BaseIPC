
#include <string>
#include <util/buffer.h>


namespace ipc{

class BaseIPC{
public :

    /// @brief BaseIPC constructor.
    ///
    /// Creates BaseIPC object that represents UNIX communication.
	BaseIPC();

	/// @brief BaseIPC destructor.
    ///
    /// Delete a BaseIPC object.
	~BaseIPC();

	/// @brief Opens UNIX sockets act as client to connect a server to pass the 
	/// packets back and forth between two processes. 
    ///
    /// Method will throw if socket creation, socket connect fails.
	/// 
    /// @param str name of socket object or address.
	///
	/// @param is_send 1 indicates send data,0 indicates receive data.
	///
    /// @return socket descriptor, if socket creation, binding were successful.
	int openSocket(const std::string& socket_name,const int is_send);

	/// @brief Closes all open sockets on interface.
	void closeSocket();

	/// @brief Send message to a host that the same socket are binding. 
	/// 
    /// @param buf the date are prepared to send.
	///
	/// @return the number of data have been sent.
	int send(const isc::util::OutputBuffer &buf);

	/// @brief receive message from a host that the same socket are binding .
	/// 
	/// @return the data have been received.
	isc::util::InputBuffer receive();

	/// @brief get socket value.
	/// 
    /// @return the socket value this class is using.
	int getsocket();

	/// @briefReturns value of transaction-id field
    ///
    /// @return transaction-id
    ///uint32_t getTransid() const { return (transid_); };

protected:

    /// UNIX socket value.
	int socketfd_;

	//we use 0 indicates send,and 1 indicates reveive.
	int is_send;

};

}//namespace ipc








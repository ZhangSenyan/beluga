#include "beluga/TCPClient.h"
#include "beluga/net/Channel.h"
#include "beluga/thread/EventLoop.h"

//Ping-Pong Client
class PPClient{
public:
    PPClient(char * ip, int port,EventLoop* eventLoop,int blockSize):
            _tcpClient(ip,port),
            _channel(new Channel(0)),
            _eventLoop(eventLoop){

        _channel->setFD(_tcpClient.getFD());

        _channel->setReadHandler(std::bind(&PPClient::handle_read,this));
        _channel->setWriteHandler(std::bind(&PPClient::handle_write,this));
        _channel->setErrorHandler(std::bind(&PPClient::handle_error,this));

        _channel->setEvents(EPOLLOUT |EPOLLIN| EPOLLET| EPOLLERR);


        for (int i = 0; i < blockSize; ++i)
        {
            _msg.push_back(static_cast<char>(i % 10)+'0');
        }
        eventLoop->addChannel(_channel);

    }
    void handle_read(){
        //std::cout<<"handle_read()"<<std::endl;
        _tcpClient.readStream();
        _channel->addEvents(EPOLLOUT);
        _eventLoop->getEpoll()->updateChannel(_channel);
    }
    void handle_write(){
        //std::cout<<"handle_write()"<<std::endl;
        _tcpClient.write(_msg);
        _channel->removeEvents(EPOLLOUT);
        _eventLoop->getEpoll()->updateChannel(_channel);
    }
    void handle_error(){
        std::cout<<"handle_error()"<<std::endl;
    }

private:
    std::shared_ptr<Channel> _channel;
    std::string _msg;
    EventLoop* _eventLoop;
    TCPClient _tcpClient;
};
typedef std::shared_ptr<PPClient> PPClientPtr;
typedef std::set<PPClientPtr> PPClientSet;
int main(){

    int clientNum=10000;

    EventLoop eventLoop;
    eventLoop.startLoop();
    PPClientSet ppClientSet;
    for(int i=0;i<clientNum;i++){
        PPClientPtr ppClientPtr(new PPClient("10.20.4.5",10000,&eventLoop,128));
        ppClientSet.insert(ppClientPtr);
        usleep(20);
    }
    while(eventLoop.isRuning()){
        sleep(1);
    }


}
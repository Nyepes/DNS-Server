
#include "Parser.hpp"
/*Start with Slow IO*/
class Resolver {
    public:
    /*Start Sever TODO: Singleton*/
    Resolver();
    void startResolver();
    void closeResolver();

    protected:
    void acceptClient();
    void handleClient();
    Packet getClientPacket();
    Packet getClientRespones();
};
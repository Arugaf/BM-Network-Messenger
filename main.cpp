#include <iostream>
#include "physical_layer/PhysicalLayer.h"

//#include "Controller.h"
//#include "Application.h"

//TODO:: transformator, кадры, контроль и обработка ошибок, управление соединением, интерфейсы, наследники


class DatalinkLayerStub : virtual public IDatalink{
    typedef char byte;
public:
    void sendData(const byte *data) override{
        std::cout<<"received : "<<data;
    }
    void connLostCallback(const PortType &portType) override{
        std::cout<<"Connection "<<portType<<" is losted"<<std::endl;
    }
    void bindPhysicalLayer(IDatalink *receiver = nullptr){
        this->physicalLayer=receiver;
    }

private:
    IDatalink *physicalLayer = nullptr;
};



int main() {
    /*std::string kekus;

    std::cin >> kekus;

    BM_Network::IFrame* frame = new BM_Network::Frame(31, 3, 1, kekus.size(), kekus.c_str());

    dynamic_cast<BM_Network::Frame*>(frame)->visualize();

    std::cout << std::endl;

    BM_Network::HammingEncoder encode(frame->getFrame(), frame->getSize());
    encode.visualize();
    std::cout << std::endl;
    encode.spoilByte(20);

    BM_Network::HammingDecoder<BM_Network::byte> decode(encode.getCodedBytes());
    decode.visualize();
    std::cout << std::endl;

    std::cout << std::string(decode.getDecodedBytes()) << std::endl;
    std::cout << std::endl;

    BM_Network::Frame frame1(decode.getDecodedBytes(), decode.getSize());
    frame1.visualize();

    std::cout << std::endl << frame1.getData();

    delete frame;*/


    //TODO сделай примерно так

//    BM_Network::ApplicationLayerController app_cl;
    //app.sendEvent(BM_Network::ACK);
    //app.sendMessage("kekus", "omegus", "cheburekus");

    PhysicalLayer phc;

    DatalinkLayerStub dl;

    //binding controllers as interfaces
    dl.bindPhysicalLayer((IDatalink *) &phc);

    phc.bindDatalinkLayer((IDatalink *) &dl);

    phc.openPort("COM2", PortType(READ));
    phc.sendData("Hello from Vova");
//    phc.bindDatalinkLayer((IPhysical*) &dl);

//    app.bindDatalinkLayer()

//    dl.bindPhysicalLayer()
//    dl.bindApplicationLayer()

//    Application application(phc, dl, app);

    //тут какая-то логика старта всей системы, управления жизнью и т.п., т.к мы запускаем отдельный поток, поэтому тут мы должны как-то застопаться, чтобы не грохнуть его
//    application.start();

    return 0;
}
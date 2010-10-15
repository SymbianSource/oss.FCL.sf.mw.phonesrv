#include <QObject>

class XQAiwRequest : public QObject
{
    Q_OBJECT
public:
    ~XQAiwRequest();
    void send();	
    void setSynchronous(bool synchronous);
    
signals:
    void requestOk(const QVariant& result);
    void requestError(int errorCode, const QString& errorMessage);
};

class XQApplicationManager
{
public:
    XQAiwRequest* create(const QString& service, const QString& interface, const QString& operation, bool embedded = true);
};


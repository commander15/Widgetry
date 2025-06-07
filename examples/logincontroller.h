#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <DataGate/abstractloginmanager.h>

class LoginController : public DataGate::AbstractLoginManager
{
public:
    LoginController();

protected:
    void attemptLogIn(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback) override;
    void attemptLogOut(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback) override;
};

#endif // LOGINCONTROLLER_H

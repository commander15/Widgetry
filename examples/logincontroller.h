#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <DataGate/abstractloginmanager.h>

class LoginController : public DataGate::AbstractLoginManager
{
public:
    LoginController();

protected:
    void attemptLogIn(const DataGate::LoginRequest &query, const DataGate::DataResponseCallback &callback) override;
    void attemptLogOut(const DataGate::LoginRequest &query, const DataGate::DataResponseCallback &callback) override;
};

#endif // LOGINCONTROLLER_H

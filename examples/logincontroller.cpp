#include "logincontroller.h"

#include <DataGate/authenticator.h>

#include <QtCore/qtimer.h>

LoginController::LoginController()
{
}

void LoginController::attemptLogIn(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback)
{
    static const QHash<QString, QString> credentials = {
        { "admin", "admin" }
    };

    const QString login = query.identifier();
    const QString password = query.password();

    DataGate::DataResponse response;
    response.setSuccess(credentials.value(login) == password);
    response.setCode(DataGate::AuthenticationError::BadCredentials);

    QTimer::singleShot(2000, [callback, response] { callback(response); });
}

void LoginController::attemptLogOut(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback)
{
    DataGate::DataResponse response;
    response.setSuccess(true);
    callback(response);
}

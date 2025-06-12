#include "logincontroller.h"

#include <DataGate/authenticator.h>

#include <QtCore/qtimer.h>

LoginController::LoginController()
{
}

void LoginController::attemptLogIn(const DataGate::LoginRequest &query, const DataGate::DataResponseCallback &callback)
{
    static const QHash<QString, QString> credentials = {
        { "admin", "admin" }
    };

    DataGate::DataResponse response;
    if (credentials.value(query.identifier()) == query.password()) {
        response.setSuccess(true);
        response.setCode(DataGate::AuthenticationError::NoError);
    } else {
        response.setSuccess(false);
        response.setCode(DataGate::AuthenticationError::BadCredentials);
    }

    QTimer::singleShot(2000, qApp, [callback, response] { callback(response); });
}

void LoginController::attemptLogOut(const DataGate::LoginRequest &query, const DataGate::DataResponseCallback &callback)
{
    DataGate::DataResponse response;
    response.setSuccess(true);
    callback(response);
}

#include "api_manage.hpp"
#include <algorithm>

ApiManage::ApiManage(QObject *parent)
    : QObject{parent}
{}


bool ApiManage::isValid(
    QString const &token,
    QString const &find_method
    )
{
  if (!tokens.contains(token))
    return false;
  return std::find_if(tokens[token].cbegin(), tokens[token].cend(),
                      [&find_method](const QString& method){
        return find_method == method || method == "all";
    }) != tokens[token].cend();
}

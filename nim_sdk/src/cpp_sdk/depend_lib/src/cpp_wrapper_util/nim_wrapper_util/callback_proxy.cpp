#include "callback_proxy.h"

namespace nim {
template <>
void CallbackProxy::Run<void>(const CallbackProxyClosure<void>& task) {
    if (docallback_async_ != nullptr) {
        docallback_async_(task);
    } else {
        task();
    }
}
}  // namespace nim
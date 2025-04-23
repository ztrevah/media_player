#ifndef HOME_CONTROLLER_H
#define HOME_CONTROLLER_H

#include "BaseController.h"

class HomeController : public BaseController {
private:
    
public:
    HomeController();
    ~HomeController();
    void openView(IView* previousView = nullptr);
    void closeView();
    void openMediaFileListView();
    void openPlaylistView();
    void openMediaPlayView();
};

#endif
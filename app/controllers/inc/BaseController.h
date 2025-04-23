#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

class IView;

class BaseController {
protected:
    IView* view;
public:
    BaseController();
    virtual ~BaseController();
    virtual void openView(IView* previousView = nullptr) = 0;
    virtual void closeView() = 0;
};

#endif
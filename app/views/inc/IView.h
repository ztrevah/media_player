#ifndef I_VIEW_H
#define I_VIEW_H

class BaseController;

class IView {
protected:
    BaseController* controller;
    IView* previousView;
    bool isDisplaying;
public:
    IView(BaseController* controller, IView* previousView = nullptr);
    virtual ~IView();
    virtual void display() = 0;

    void setPreviousView(IView* previousView);
    IView* getPreviousView();
};

#endif
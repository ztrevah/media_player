#include "../inc/IView.h"

IView::IView(BaseController* controller, IView* previousView) {
    this->controller = controller;
    this->previousView = previousView;
    this->isDisplaying = false;
}

IView::~IView() {

}

void IView::setPreviousView(IView* previousView) {
    this->previousView = previousView;
}
IView* IView::getPreviousView() {
    return this->previousView;
}
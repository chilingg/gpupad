#ifndef RRESOURCEWINDOW_H
#define RRESOURCEWINDOW_H

#include "RWindowCtrl.h"
#include "RResource/RResource.h"
#include "RTextPlane.h"
#include "RTimer.h"

class RResourceWindow : public RWindowCtrl
{
public:
    explicit RResourceWindow(const std::string &name = "ResourceWindow", RController *parent = nullptr);
    ~RResourceWindow() override;

    void control() override;
    void updateReSourceList();
    void updateControllerTree(RController* node, RPoint2 pos);

protected:
    std::string getDefaultName() const override;
    void initEvent(RInitEvent *event) override;
    void closeEvent(RCloseEvent *event) override;
    void resizeEvent(RResizeEvent *event) override;
    void scrollEvent(RScrollEvent *event) override;
    void inputEvent(RInputEvent *event) override;

private:
    void checkScrollBarOffer();

    RShaderProgram windowShaders_;
    RPlane rcListBackground_;
    RTextPlane rcTitle_;
    int rcWidgetWidth_;
    RTextPlane rcListPlane_;
    int rcCount_;
    const RResource::ResourcesList *resourceList;
    RTextPlane rcCountPlane_;
    int rcListHResizeRange_ = 3;

    RPlane rcScrollBar_;
    RPlane rcScrollBarBack_;
    int rcListPlaneOffsetY_ = 0;
    int rcScrollBarOffsetY_ = 0;

    RTextPlane ctrlTreeTitle_;
    RTextPlane nodeLabel_;

    RCursor cursor_;

    RTimer timer_;
};

#endif // RRESOURCEWINDOW_H

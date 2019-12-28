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
    void translation(const TranslationInfo &info) override;

    void updateReSourceList();
    int updateControllerTree(RController* node, RPoint2 pos);

protected:
    std::string getDefaultName() const override;
    void startEvent(RStartEvent *event) override;
    void finishEvent(RFinishEvent *event) override;
    void scrollEvent(RScrollEvent *event) override;
    void inputEvent(RInputEvent *event) override;

private:
    static constexpr GLint zero = 0;

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

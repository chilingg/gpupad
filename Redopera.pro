TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../Redopera/Redlib
INCLUDEPATH += $$PWD/../Redopera/Redlib/Extern

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/bin_debug
    DEFINES += R_DEBUG
} else {
    DESTDIR = bin_release
}

unix:!macx:{
LIBS += -lglfw \
    -ldl \
    -L/usr/lib/ -lfreetype

INCLUDEPATH += /usr/include/freetype2
}

SOURCES += \
        Learn.cpp \
        Redlib/RResource/RCursor.cpp \
        Redlib/Extern/Tonic/ADSR.cpp \
        Redlib/Extern/Tonic/Arithmetic.cpp \
        Redlib/Extern/Tonic/AudioFileUtils.cpp \
        Redlib/Extern/Tonic/BLEPOscillator.cpp \
        Redlib/Extern/Tonic/BasicDelay.cpp \
        Redlib/Extern/Tonic/BitCrusher.cpp \
        Redlib/Extern/Tonic/BufferFiller.cpp \
        Redlib/Extern/Tonic/BufferPlayer.cpp \
        Redlib/Extern/Tonic/CombFilter.cpp \
        Redlib/Extern/Tonic/CompressorLimiter.cpp \
        Redlib/Extern/Tonic/ControlArithmetic.cpp \
        Redlib/Extern/Tonic/ControlCallback.cpp \
        Redlib/Extern/Tonic/ControlChangeNotifier.cpp \
        Redlib/Extern/Tonic/ControlComparison.cpp \
        Redlib/Extern/Tonic/ControlConditioner.cpp \
        Redlib/Extern/Tonic/ControlCounter.cpp \
        Redlib/Extern/Tonic/ControlDbToLinear.cpp \
        Redlib/Extern/Tonic/ControlDelay.cpp \
        Redlib/Extern/Tonic/ControlFloor.cpp \
        Redlib/Extern/Tonic/ControlGenerator.cpp \
        Redlib/Extern/Tonic/ControlMetro.cpp \
        Redlib/Extern/Tonic/ControlMetroDivider.cpp \
        Redlib/Extern/Tonic/ControlMidiToFreq.cpp \
        Redlib/Extern/Tonic/ControlParameter.cpp \
        Redlib/Extern/Tonic/ControlPrinter.cpp \
        Redlib/Extern/Tonic/ControlPulse.cpp \
        Redlib/Extern/Tonic/ControlRandom.cpp \
        Redlib/Extern/Tonic/ControlRecorder.cpp \
        Redlib/Extern/Tonic/ControlSnapToScale.cpp \
        Redlib/Extern/Tonic/ControlStepper.cpp \
        Redlib/Extern/Tonic/ControlSwitcher.cpp \
        Redlib/Extern/Tonic/ControlTrigger.cpp \
        Redlib/Extern/Tonic/ControlTriggerFilter.cpp \
        Redlib/Extern/Tonic/ControlValue.cpp \
        Redlib/Extern/Tonic/ControlXYSpeed.cpp \
        Redlib/Extern/Tonic/DSPUtils.cpp \
        Redlib/Extern/Tonic/DelayUtils.cpp \
        Redlib/Extern/Tonic/Effect.cpp \
        Redlib/Extern/Tonic/FilterUtils.cpp \
        Redlib/Extern/Tonic/Filters.cpp \
        Redlib/Extern/Tonic/FixedValue.cpp \
        Redlib/Extern/Tonic/Generator.cpp \
        Redlib/Extern/Tonic/LFNoise.cpp \
        Redlib/Extern/Tonic/Mixer.cpp \
        Redlib/Extern/Tonic/MonoToStereoPanner.cpp \
        Redlib/Extern/Tonic/Noise.cpp \
        Redlib/Extern/Tonic/RampedValue.cpp \
        Redlib/Extern/Tonic/RectWave.cpp \
        Redlib/Extern/Tonic/Reverb.cpp \
        Redlib/Extern/Tonic/RingBuffer.cpp \
        Redlib/Extern/Tonic/SampleTable.cpp \
        Redlib/Extern/Tonic/SawtoothWave.cpp \
        Redlib/Extern/Tonic/SineWave.cpp \
        Redlib/Extern/Tonic/StereoDelay.cpp \
        Redlib/Extern/Tonic/Synth.cpp \
        Redlib/Extern/Tonic/TableLookupOsc.cpp \
        Redlib/Extern/Tonic/TonicFrames.cpp \
        Redlib/RResourceWindow.cpp \
        Redlib/RSprite.cpp \
        Redlib/Extern/SourceCodePro.cpp \
        Redlib/Extern/glad.c \
        Redlib/Extern/stb_image_implementatio.cpp \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RInputEvent.cpp \
        Redlib/RPlane.cpp \
        Redlib/RResource/RFont.cpp \
        Redlib/RResource/RImage.cpp \
        Redlib/RResource/RResource.cpp \
        Redlib/RResource/RShader.cpp \
        Redlib/RResource/RShaderProgram.cpp \
        Redlib/RResource/RTexture.cpp \
        Redlib/RTextPlane.cpp \
        Redlib/RWindowCtrl.cpp \
        Testctr.cpp \
        main.cpp

HEADERS += \
    Learn.h \
    Redlib/RRect.h \
    Redlib/RResource/RCursor.h \
    Redlib/Extern/Tonic.h \
    Redlib/Extern/Tonic/ADSR.h \
    Redlib/Extern/Tonic/Arithmetic.h \
    Redlib/Extern/Tonic/AudioFileUtils.h \
    Redlib/Extern/Tonic/BLEPOscillator.h \
    Redlib/Extern/Tonic/BasicDelay.h \
    Redlib/Extern/Tonic/BitCrusher.h \
    Redlib/Extern/Tonic/BufferFiller.h \
    Redlib/Extern/Tonic/BufferPlayer.h \
    Redlib/Extern/Tonic/CombFilter.h \
    Redlib/Extern/Tonic/CompressorLimiter.h \
    Redlib/Extern/Tonic/ControlArithmetic.h \
    Redlib/Extern/Tonic/ControlCallback.h \
    Redlib/Extern/Tonic/ControlChangeNotifier.h \
    Redlib/Extern/Tonic/ControlComparison.h \
    Redlib/Extern/Tonic/ControlConditioner.h \
    Redlib/Extern/Tonic/ControlCounter.h \
    Redlib/Extern/Tonic/ControlDbToLinear.h \
    Redlib/Extern/Tonic/ControlDelay.h \
    Redlib/Extern/Tonic/ControlFloor.h \
    Redlib/Extern/Tonic/ControlGenerator.h \
    Redlib/Extern/Tonic/ControlMetro.h \
    Redlib/Extern/Tonic/ControlMetroDivider.h \
    Redlib/Extern/Tonic/ControlMidiToFreq.h \
    Redlib/Extern/Tonic/ControlParameter.h \
    Redlib/Extern/Tonic/ControlPrinter.h \
    Redlib/Extern/Tonic/ControlPulse.h \
    Redlib/Extern/Tonic/ControlRandom.h \
    Redlib/Extern/Tonic/ControlRecorder.h \
    Redlib/Extern/Tonic/ControlSnapToScale.h \
    Redlib/Extern/Tonic/ControlStepper.h \
    Redlib/Extern/Tonic/ControlSwitcher.h \
    Redlib/Extern/Tonic/ControlTrigger.h \
    Redlib/Extern/Tonic/ControlTriggerFilter.h \
    Redlib/Extern/Tonic/ControlValue.h \
    Redlib/Extern/Tonic/ControlXYSpeed.h \
    Redlib/Extern/Tonic/DSPUtils.h \
    Redlib/Extern/Tonic/DelayUtils.h \
    Redlib/Extern/Tonic/Effect.h \
    Redlib/Extern/Tonic/FilterUtils.h \
    Redlib/Extern/Tonic/Filters.h \
    Redlib/Extern/Tonic/FixedValue.h \
    Redlib/Extern/Tonic/Generator.h \
    Redlib/Extern/Tonic/LFNoise.h \
    Redlib/Extern/Tonic/Mixer.h \
    Redlib/Extern/Tonic/MonoToStereoPanner.h \
    Redlib/Extern/Tonic/Noise.h \
    Redlib/Extern/Tonic/RampedValue.h \
    Redlib/Extern/Tonic/RectWave.h \
    Redlib/Extern/Tonic/Reverb.h \
    Redlib/Extern/Tonic/RingBuffer.h \
    Redlib/Extern/Tonic/SampleTable.h \
    Redlib/Extern/Tonic/SawtoothWave.h \
    Redlib/Extern/Tonic/SineWave.h \
    Redlib/Extern/Tonic/SquareWave.h \
    Redlib/Extern/Tonic/StereoDelay.h \
    Redlib/Extern/Tonic/Synth.h \
    Redlib/Extern/Tonic/TableLookupOsc.h \
    Redlib/Extern/Tonic/TonicCore.h \
    Redlib/Extern/Tonic/TonicFrames.h \
    Redlib/Extern/Tonic/TriangleWave.h \
    Redlib/RResourceWindow.h \
    Redlib/RSprite.h \
    Redlib/Extern/sigslot.h \
    Redlib/Extern/stb_image.h \
    Redlib/RColor.h \
    Redlib/RController.h \
    Redlib/RDebug.h \
    Redlib/RInputEvent.h \
    Redlib/RMaths.h \
    Redlib/RNotifyEvent.h \
    Redlib/ROpenGL.h \
    Redlib/RPlane.h \
    Redlib/RPoint.h \
    Redlib/RResource/RFont.h \
    Redlib/RResource/RImage.h \
    Redlib/RResource/RResource.h \
    Redlib/RResource/RShader.h \
    Redlib/RResource/RShaderProgram.h \
    Redlib/RResource/RTexture.h \
    Redlib/RSize.h \
    Redlib/RTextPlane.h \
    Redlib/RTimer.h \
    Redlib/RWindowCtrl.h \
    Redlib/RSigslot.h \
    Testctr.h

DISTFILES += \
    Resource/StackList.txt \
    Resource/shader/Fragment.frag \
    Resource/shader/LineBox.frag \
    Resource/shader/LineBox.geom \
    Resource/shader/LineBox.vert \
    Resource/shader/Vertex.vert

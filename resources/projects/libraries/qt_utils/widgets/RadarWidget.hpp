/*
 * Description:  Widget displaying a webots radar device
 */

#ifndef RADAR_WIDGET_HPP
#define RADAR_WIDGET_HPP

#include "SensorWidget.hpp"

class QLabel;
class QHBoxLayout;

namespace webotsQtUtils {
  class Graph2D;

  class RadarWidget : public SensorWidget {
    Q_OBJECT

  public:
    RadarWidget(Device *device, QWidget *parent = NULL);
    virtual ~RadarWidget() {}

    virtual void readSensors();

  protected slots:
    virtual void enable(bool enable);

  protected:
    bool isEnabled() const;

    QLabel *mTargetNumberLabel;
    Graph2D *mGraph2D;
    QHBoxLayout *mHBox;
  };
}  // namespace webotsQtUtils

#endif

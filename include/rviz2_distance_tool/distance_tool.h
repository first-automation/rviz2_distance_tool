#ifndef RVIZ2_DISTANCE_TOOL_H
#define RVIZ2_DISTANCE_TOOL_H

#include <rviz_common/tool.hpp>
#include <rviz_rendering/objects/line.hpp>
#include <rviz_common/load_resource.hpp>
#include <Ogre.h>
#include <QCursor>
#include <array>

namespace rviz2_distance_tool
{

class DistanceTool : public rviz_common::Tool
{
  Q_OBJECT
public:
  DistanceTool();
  ~DistanceTool() override;

  void onInitialize() override;

  void activate() override;

  void deactivate() override;

  int processMouseEvent(rviz_common::ViewportMouseEvent& event) override;

private:

  void configureLines(const Ogre::Vector3& start, const Ogre::Vector3& end);
  void configureStatus(const Ogre::Vector3& start, const Ogre::Vector3& end);
  void hideLines();

  enum class SelectionState {
    Idle, Tracking, Finished
  } state_;

  std::array<rviz_rendering::Line*, 4> lines_;
  Ogre::Vector3 start_;
  Ogre::Vector3 end_;

  QCursor std_cursor_;
  QCursor hit_cursor_;
};

} // namespace rviz2_distance_tool

#endif // RVIZ2_DISTANCE_TOOL_H

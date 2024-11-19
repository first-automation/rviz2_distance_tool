#include "rviz2_distance_tool/distance_tool.h"

#include <pluginlib/class_list_macros.hpp>

#include <rviz_common/display_context.hpp>
#include <rviz_common/interaction/view_picker.hpp>
#include <rviz_common/viewport_mouse_event.hpp>
#include <rviz_rendering/objects/line.hpp>

#include <Ogre.h>

#include <QCursor>
#include <sstream>

namespace rviz2_distance_tool
{

DistanceTool::DistanceTool()
  : state_(SelectionState::Idle)
  , lines_{{nullptr, nullptr, nullptr, nullptr}}
{}

DistanceTool::~DistanceTool()
{
  for (auto& line : lines_) delete line;
}

void DistanceTool::onInitialize()
{
  for (auto& line : lines_)
  {
    line = new rviz_rendering::Line(context_->getSceneManager(), context_->getSceneManager()->getRootSceneNode());
    line->setVisible(false);
  }

  lines_[0]->setColor(1.0f, 1.0f, 1.0f, 1.0f); // 総距離
  lines_[1]->setColor(1.0f, 0.0f, 0.0f, 1.0f); // X軸
  lines_[2]->setColor(0.0f, 1.0f, 0.0f, 1.0f); // Y軸
  lines_[3]->setColor(0.0f, 0.0f, 1.0f, 1.0f); // Z軸

  std_cursor_ = rviz_common::getDefaultCursor();
  hit_cursor_ = rviz_common::getDefaultCursor(); // 必要に応じて変更

  shortcut_key_ = 'd';
}

void DistanceTool::activate()
{}

void DistanceTool::deactivate()
{
  hideLines();
  setStatus("");
}

int DistanceTool::processMouseEvent(rviz_common::ViewportMouseEvent& event)
{
  int flags = 0;

  Ogre::Vector3 pos;
  bool success = context_->getViewPicker()->get3DPoint(event.panel, event.x, event.y, pos);
  setCursor(success ? hit_cursor_ : std_cursor_);

  switch (state_)
  {
  case SelectionState::Idle:
    if (success && event.leftDown())
    {
      start_ = pos;
      state_ = SelectionState::Tracking;
    }
    break;

  case SelectionState::Tracking:
    if (success)
    {
      configureLines(start_, pos);
      configureStatus(start_, pos);

      if (event.leftDown())
      {
        end_ = pos;
        state_ = SelectionState::Finished;
      }
    }
    flags |= Render;
    break;

  case SelectionState::Finished:
    configureStatus(start_, end_);

    if (event.leftDown())
    {
      start_ = pos;
      state_ = SelectionState::Tracking;
    }
    flags |= Render;
    break;
  }

  if (event.rightUp())
  {
    state_ = SelectionState::Idle;
    start_ = end_ = Ogre::Vector3::ZERO;
    hideLines();
    setStatus("");
  }

  return flags;
}

void DistanceTool::configureLines(const Ogre::Vector3 &start, const Ogre::Vector3 &end)
{
  const Ogre::Vector3 delta = end - start;

  lines_[0]->setPoints(start, end);
  lines_[0]->setVisible(true);

  Ogre::Vector3 start_pt = start;
  Ogre::Vector3 end_pt = start_pt + Ogre::Vector3(delta.x, 0.0, 0.0);
  lines_[1]->setPoints(start_pt, end_pt);
  lines_[1]->setVisible(true);

  start_pt = end_pt;
  end_pt = end_pt + Ogre::Vector3(0, delta.y, 0);
  lines_[2]->setPoints(start_pt, end_pt);
  lines_[2]->setVisible(true);

  start_pt = end_pt;
  end_pt = end_pt + Ogre::Vector3(0, 0, delta.z);
  lines_[3]->setPoints(start_pt, end_pt);
  lines_[3]->setVisible(true);
}

void DistanceTool::configureStatus(const Ogre::Vector3 &start, const Ogre::Vector3 &end)
{
  const Ogre::Vector3 delta = end - start;
  std::ostringstream oss;
  oss << "Distance = " << delta.length()
      << " [x = " << delta.x
      << ", y = " << delta.y
      << ", z = " << delta.z << "]";
  setStatus(QString::fromStdString(oss.str()));
}

void DistanceTool::hideLines()
{
  for (auto& line : lines_)
    line->setVisible(false);
}

} // namespace rviz2_distance_tool

PLUGINLIB_EXPORT_CLASS(rviz2_distance_tool::DistanceTool, rviz_common::Tool)

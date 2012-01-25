/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "agent.hpp"
#include "sphinx.hpp"

using namespace SpeechControl;
using namespace SpeechControl::DesktopControl;

/// @todo #issue 0000032
Agent::Agent(QObject* p_prnt) : QObject(p_prnt)
{
  s_inst = this;
  m_sphnx = new Sphinx;
}

Agent::Agent(const Agent& p_other) : QObject(p_other.parent()),
    m_sphnx(p_other.m_sphnx)
{

}

Agent* Agent::instance()
{
  if (!s_inst)
    s_inst = new Agent;

  return s_inst;
}

void Agent::start()
{
    connect(s_inst , SIGNAL(started()) , s_inst->m_sphnx,SLOT(startRecognizing()));
    emit s_inst->started();
}

void Agent::stop()
{
  emit s_inst->stopped();
}

Agent::~Agent()
{

}

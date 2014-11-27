/*
  Copyright (c) 2014, J.D. Koftinoff Software, Ltd.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   3. Neither the name of J.D. Koftinoff Software, Ltd. nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include "JDKSAvdeccMCU_World.hpp"

#include "JDKSAvdeccMCU_RawSocket.hpp"
#include "JDKSAvdeccMCU_Frame.hpp"
#include "JDKSAvdeccMCU_Handler.hpp"
#include "JDKSAvdeccMCU_Helpers.hpp"
#include "JDKSAvdeccMCU_Entity.hpp"
#include "JDKSAvdeccMCU_ControllerEntity.hpp"
#include "JDKSAvdeccMCU_ControlValueHolder.hpp"

namespace JDKSAvdeccMCU
{
class Control : public Handler
{
  public:
    /// Construct the SetControlSender object
    Control( Entity &entity,
             uint16_t descriptor_index,
             Eui64 control_type,
             uint16_t control_value_type,
             ControlValueHolder *holder );

    virtual void tick( jdksavdecc_timestamp_in_milliseconds time_in_millis );

    virtual bool receivedPDU( Frame &frame );

    virtual uint8_t formControlValueMetaData( Frame &pdu );

    virtual uint8_t formControlPayload( Frame &pdu );

    virtual uint8_t validateSetControlCommand( Frame &pdu );

    virtual uint8_t formSetControlResponse( Frame &pdu );

    virtual uint8_t formGetControlCommand( Frame &pdu );

    virtual uint8_t formGetControlResponse( Frame &pdu );

    virtual uint8_t readControlDescriptor( Frame &pdu );

    Entity &getEntity() { return m_entity; }

    Entity const &getEntity() const { return m_entity; }

    ControlValueHolder *getControlValueHolder() { return m_holder; }

    ControlValueHolder const *getControlValueHolder() const { return m_holder; }

  protected:
    Entity &m_entity;
    uint16_t m_descriptor_index;
    Eui64 m_control_type;
    uint16_t m_control_value_type;
    ControlValueHolder *m_holder;
};
}

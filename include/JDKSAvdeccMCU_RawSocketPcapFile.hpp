#pragma once

/*
Copyright (c) 2014, Jeff Koftinoff
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "JDKSAvdeccMCU_World.hpp"
#include "JDKSAvdeccMCU_RawSocket.hpp"
#include "JDKSAvdeccMCU_PcapFileReader.hpp"
#include "JDKSAvdeccMCU_PcapFileWriter.hpp"

namespace JDKSAvdeccMCU
{

class RawSocketPcapFile : public RawSocket
{
    uint16_t m_ethertype;
    jdksavdecc_eui48 m_my_mac;
    jdksavdecc_eui48 m_default_dest_mac;

  public:
    /**
    *  Open a raw socket connected to the specified interface name and join the
    *  specified multicast address
    */
    RawSocketPcapFile( uint16_t ethertype,
                       const char *input_file,
                       const char *output_file );

    ~RawSocketPcapFile();

    virtual jdksavdecc_timestamp_in_milliseconds getTimeInMilliseconds();

    virtual bool recvFrame( FrameBase *frame );

    virtual bool sendFrame( FrameBase const &frame,
                            uint8_t const *data1,
                            uint16_t len1,
                            uint8_t const *data2,
                            uint16_t len2 );

    virtual bool sendReplyFrame( FrameBase &frame,
                                 uint8_t const *data1,
                                 uint16_t len1,
                                 uint8_t const *data2,
                                 uint16_t len2 );

    virtual bool joinMulticast( const jdksavdecc_eui48 &multicast_mac );

    virtual void setNonblocking();

    virtual filedescriptor_t getFd() const;

    virtual jdksavdecc_eui48 const &getMACAddress() const;
};
}

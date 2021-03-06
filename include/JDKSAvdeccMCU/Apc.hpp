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

#include "JDKSAvdeccMCU/World.hpp"
#include "JDKSAvdeccMCU/AppMessage.hpp"
#include "JDKSAvdeccMCU/AppMessageParser.hpp"
#include "JDKSAvdeccMCU/AppMessageHandler.hpp"
#include "JDKSAvdeccMCU/Http.hpp"

namespace JDKSAvdeccMCU
{

class ApcStateVariables;
class ApcStateActions;
class ApcStates;
class ApcStateEvents;
class ApcStateMachine;

class ApcStateMachine
{
  public:

    ApcStateMachine( ApcStateVariables *variables, ApcStateActions *actions, ApcStateEvents *events, ApcStates *states );

    virtual ~ApcStateMachine();

    virtual void setup();
    virtual void clear();

    void setPrimaryMac( Eui48 mac );
    void setEntityId( Eui64 const entity_id );
    Eui64 getEntityId() const;
    void setApsAddress( std::string const &addr );
    void setPath( std::string const &path );

    virtual bool run();

    virtual void onTimeTick( uint32_t time_in_seconds );
    virtual void onTcpConnectionClosed();
    virtual void closeTcpConnection();
    virtual void connectToProxy( std::string const &addr );
    virtual void notifyProxyAvailable();
    virtual void notifyProxyUnavailable();
    virtual void notifyLinkStatus( AppMessage const &linkMsg );
    virtual void processMsg( AppMessage const &apsMsg );
    virtual void notifyNewEntityId( Eui64 const &entity_id );
    virtual ssize_t onIncomingTcpData( uint8_t const *data, ssize_t len );

    virtual void sendTcpData( uint8_t const *data, ssize_t len );
    virtual void onNetAvdeccMessageReceived( Frame const &frame );

    ApcStateVariables *getVariables() { return m_variables; }
    ApcStateVariables const *getVariables() const { return m_variables; }
    ApcStateActions *getActions() { return m_actions; }
    ApcStateActions const *getActions() const { return m_actions; }
    ApcStateEvents *getEvents() { return m_events; }
    ApcStateEvents const *getEvents() const { return m_events; }
    ApcStates *getStates() { return m_states; }
    ApcStates const *getStates() const { return m_states; }

  protected:
    ApcStateVariables *m_variables;
    ApcStateActions *m_actions;
    ApcStateEvents *m_events;
    ApcStates *m_states;
};

class ApcStateVariables
{
  public:
    virtual ~ApcStateVariables();

    ///
    /// \brief setOwner set the owner
    /// \param owner ApsStateMachine
    ///
    virtual void setOwner( ApcStateMachine *owner ) { m_owner = owner; }

    ///
    /// \brief clear all variables
    ///
    virtual void clear();

    ///
    /// \brief m_owner
    ///
    /// The ApsStateMachine that owns these variables
    ///
    ApcStateMachine *m_owner;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.1
     * Contains the host and port of the APS
     */
    std::string m_addr;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.1
     * Contains the full HTTP Request
     */
    HttpRequest m_request;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.2
     */
    AppMessage m_apcMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.3
     */
    bool m_apcMsgOut;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.4
     */
    AppMessage m_apsMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.5
     */
    bool m_apsMsgIn;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.6
     */
    uint32_t m_currentTime;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.7
     */
    bool m_finished;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.8
     */
    Eui64 m_entityId;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.9
     */
    bool m_idAssigned;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.10
     */
    bool m_incomingTcpClosed;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.11
     */
    AppMessage m_linkMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.12
     */
    bool m_linkStatusMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.13
     */
    Eui64 m_newId;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.14
     */
    uint32_t m_nopTimeout;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.15
     */
    Eui48 m_primaryMac;

    /**
     * See IEEE 1722.1 Cor 1 Annex C
     */
    bool m_responseReceived;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.16
     */
    bool m_responseValid;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.17
     */
    bool m_tcpConnected;
};

class ApcStateActions
{
  public:
    ApcStateActions() {}
    virtual ~ApcStateActions() {}

    ///
    /// \brief setOwner Sets the owner
    /// \param owner ApsStateMachine
    ///
    virtual void setOwner( ApcStateMachine *owner ) { m_owner = owner; }

    ///
    /// \brief getOwner gets the owner
    /// \return ApsStateMachine
    ///
    ApcStateMachine *getOwner() { return m_owner; }

    ///
    /// \brief getVariables
    ///
    /// Get the state machine variables
    ///
    /// \return StateVariables
    ///
    ApcStateVariables *getVariables() { return m_owner->getVariables(); }

    ///
    /// \brief getEvents
    ///
    /// Ask the owning state machine for the state events object
    ///
    /// \return
    ///
    ApcStateEvents *getEvents() { return m_owner->getEvents(); }

    ///
    /// \brief clear any additional
    /// state
    ///
    virtual void clear();

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.1
     */
    virtual void closeTcpConnection();

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.2
     */
    virtual void connectToProxy( std::string const &addr );

    /**
     * See IEEE Std 1722.1 Cor1 Annex C
     */
    virtual void notifyProxyAvailable();

    /**
     * See IEEE Std 1722.1 Cor1 Annex C
     */
    virtual void notifyProxyUnavailable();

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.5
     */
    virtual void notifyLinkStatus( AppMessage const &linkMsg );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.6
     */
    virtual void processMsg( AppMessage const &apsMsg );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.X
     */
    virtual void notifyNewEntityId( Eui64 const &entity_id );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.4
     */
    virtual void initialize();

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.7
     */
    virtual void sendIdRequest( Eui48 const &primaryMac, Eui64 const &entity_id );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.8
     */
    virtual void sendHttpRequest( HttpRequest const &request );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.9
     */
    virtual void sendMsgToAps( AppMessage const &apcMsg );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.10
     */
    virtual void sendNopToAps();

  protected:
    ApcStateMachine *m_owner;
};

class ApcStates
{
  public:
    ///
    /// The state_proc member is a pointer to the
    /// current state handling procedure, or 0 if done
    ///
    typedef void ( ApcStates::*state_proc )();

    ///
    /// \brief States constructor
    ///
    /// Start the state machine in the begin state
    ///
    ApcStates() : m_owner( 0 ), m_current_state( &ApcStates::doBegin ) {}

    ///
    /// \brief ~States
    ///
    virtual ~ApcStates() {}

    ///
    /// \brief setOwner
    /// Set the owner of this state machine
    ///
    /// \param owner
    ///
    virtual void setOwner( ApcStateMachine *owner ) { m_owner = owner; }

    ///
    /// \brief getOwner
    /// get the owner
    ///
    /// \return ApsStateMachine
    ///
    ApcStateMachine *getOwner() { return m_owner; }

    ///
    /// \brief getActions
    ///
    /// Ask the owning state machine for the actions object
    ///
    /// \return StateActions
    ///
    ApcStateActions *getActions() { return m_owner->getActions(); }

    ///
    /// \brief getVariables
    ///
    /// As the owning state machine for the state variables object
    ///
    /// \return StateVariables
    ///
    ApcStateVariables *getVariables() { return m_owner->getVariables(); }

    ///
    /// \brief getEvents
    ///
    /// Ask the owning state machine for the state events object
    ///
    /// \return
    ///
    ApcStateEvents *getEvents() { return m_owner->getEvents(); }

    ///
    /// \brief clear
    /// Clear all variables and initialize the initial state
    ///
    virtual void clear();

    ///
    /// \brief run Run the state machine
    ///
    /// \return false when the state machine is complete
    ///
    virtual bool run();

  protected:
    ///
    /// \brief doBegin
    ///
    /// Implement the BEGIN state
    ///
    /// See Figure C.3 'Begin'
    ///
    virtual void doBegin();

    ///
    /// \brief goToInitialize
    ///
    /// Transition to the INITIALIZE state
    ///
    /// Perform the functions listed in Figure C.3 'INITIALIZE'
    /// state
    ///
    virtual void goToInitialize();

    ///
    /// \brief doInitialize
    ///
    /// Implement the INITIALIZE state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'INITIALIZE' state
    ///
    virtual void doInitialize();

    ///
    /// \brief goToWaitForConnect
    ///
    /// Transition to the WAIT_FOR_CONNECT state
    ///
    /// Perform the functions listed in Figure C.3,
    /// 'WAIT_FOR_CONNECT' state
    ///
    virtual void goToWaitForConnect();

    ///
    /// \brief doWaitForConnect
    ///
    /// Implement the WAIT_FOR_CONNECT state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'WAIT_FOR_CONNECT' state
    ///
    virtual void doWaitForConnect();

    ///
    /// \brief goToConnected
    ///
    /// Transition to the CONNECTED state
    ///
    /// Perform the functions listed in Figure C.3
    /// 'CONNECTED' state
    ///
    virtual void goToConnected();

    ///
    /// \brief doConnected
    ///
    /// Implement the CONNECTED state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'CONNECTED' state
    ///
    virtual void doConnected();

    ///
    /// \brief goToStartTransfer
    ///
    /// Transition to the START_TRANSFER state
    ///
    /// Perform the functions listed in Figure C.3
    /// 'START_TRANSFER' state
    ///
    virtual void goToStartTransfer();

    ///
    /// \brief doStartTransfer
    ///
    /// Implement the START_TRANSFER state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'START_TRANSFER' state
    ///
    virtual void doStartTransfer();

    ///
    /// \brief goToWaiting
    ///
    /// Transition to the WAITING state
    ///
    /// Perform the functions listed in Figure C.3
    /// 'WAITING' state
    ///
    virtual void goToWaiting();

    ///
    /// \brief doWaiting
    ///
    /// Implement the WAITING state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'WAITING' state
    ///
    virtual void doWaiting();

    ///
    /// \brief goToClosed
    ///
    /// Transition to the CLOSED state
    ///
    /// Perform the functions listed in Figur C.3
    /// 'CLOSED' state
    ///
    virtual void goToClosed();

    ///
    /// \brief doClosed
    ///
    /// Implement the CLOSED state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'CLOSED" state
    ///
    virtual void doClosed();

    ///
    /// \brief goToLinkStatus
    ///
    /// Transition to the LINK_STATUS state
    ///
    /// Perform the functions listed in Figure C.3
    /// 'LINK_STATUS' state
    ///
    virtual void goToLinkStatus();

    ///
    /// \brief doLinkStatus
    ///
    /// Implement the LINK_STATUS state
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'LINK_STATUS' state
    ///
    virtual void doLinkStatus();

    ///
    /// \brief goToReceiveMsg
    ///
    /// Transition to the RECEIVE_MSG state
    ///
    /// Peform the functions listed in Figure C.3
    /// 'RECEIVE_MSG' state
    ///
    virtual void goToReceiveMsg();

    ///
    /// \brief doReceiveMsg
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'RECEIVE_MSG' state
    ///
    virtual void doReceiveMsg();

    ///
    /// \brief goToSendMsg
    ///
    /// Transition to the SEND_MSG state
    ///
    /// Peform the functions listed in Figure C.3
    /// 'SEND_MSG' state
    ///
    virtual void goToSendMsg();

    ///
    /// \brief doSendMsg
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'SEND_MSG' state
    ///
    virtual void doSendMsg();

    ///
    /// \brief goToEntityIdAssigned
    ///
    /// Transition to the ENTITY_ID_ASSIGNED state
    ///
    /// Peform the functions listed in Figure C.3
    /// 'ENTITY_ID_ASSIGNED' state
    ///
    virtual void goToEntityIdAssigned();

    ///
    /// \brief doEntityIdAssigned
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'ENTITY_ID_ASSIGNED' state
    ///
    virtual void doEntityIdAssigned();

    ///
    /// \brief goToSendNop
    ///
    /// Transition to the SEND_NOP state
    ///
    /// Peform the functions listed in Figure C.3
    /// 'SEND_NOP' state
    ///
    virtual void goToSendNop();

    ///
    /// \brief doSendNop
    ///
    /// Perform the tests for the paths leaving Figure
    /// C.3 'SEND_NOP' state
    ///
    virtual void doSendNop();

    ///
    /// \brief goToFinish
    ///
    /// Transition to the FINISH state
    ///
    /// Perform the functions listed in Figure C.3
    /// 'FINISH' state
    ///
    virtual void goToFinish();

    ///
    /// \brief doFinish
    ///
    /// Implement the FINISH state
    ///
    ///
    virtual void doFinish();

  protected:
    ApcStateMachine *m_owner;
    state_proc m_current_state;
};

class ApcStateEvents : public AppMessageHandler, public HttpClientHandler
{
  public:
    ApcStateEvents( HttpClientParser *http_parser, std::string path )
        : m_owner( 0 ), m_http_parser( http_parser ), m_path( path ), m_app_parser( *this )
    {
    }

    virtual ~ApcStateEvents() {}

    ///
    /// \brief setOwner
    /// Set the owner of this state events object
    ///
    /// \param owner
    ///
    virtual void setOwner( ApcStateMachine *owner ) { m_owner = owner; }

    ///
    /// \brief getOwner
    /// get the owner
    ///
    /// \return ApsStateMachine
    ///
    ApcStateMachine *getOwner() { return m_owner; }

    ///
    /// \brief getActions
    ///
    /// Ask the owning state machine for the actions object
    ///
    /// \return StateActions
    ///
    ApcStateActions *getActions() { return m_owner->getActions(); }

    ///
    /// \brief getVariables
    ///
    /// As the owning state machine for the state variables object
    ///
    /// \return StateVariables
    ///
    ApcStateVariables *getVariables() { return m_owner->getVariables(); }

    ///
    /// \brief clear
    ///
    /// Clear the events object
    ///
    virtual void clear();

    ///
    /// \brief onTcpConnection
    ///
    /// Notify the state machine that a tcp connection
    /// to the APS has completed
    ///
    virtual void onIncomingTcpConnection();

    ///
    /// \brief onTcpConnectionClosed
    ///
    /// Notify the state machine that the TCP connection
    /// was closed
    ///
    ///
    virtual void onTcpConnectionClosed();

    ///
    /// \brief onIncomingTcpData
    ///
    /// Notify the state machine that some data was received
    /// from the APS
    ///
    /// \param data ptr to octets
    /// \param len lenth of data in octets
    /// \return length of consumed data
    ///
    virtual ssize_t onIncomingTcpData( uint8_t const *data, ssize_t len );

    ///
    /// \brief onNetAvdeccMessageReceived
    ///
    /// Notify the state machine that an AVDECC message
    /// destined for the APS was received
    ///
    /// \param frame Ethernet frame
    ///
    virtual void onNetAvdeccMessageReceived( Frame const &frame );

    ///
    /// \brief onTimeTick
    ///
    /// Notify the state machine that some time has passed
    /// and the new time in seconds is updated
    ///
    /// \param time_in_seconds
    ///
    virtual void onTimeTick( uint32_t time_in_seconds );

    ///
    /// \brief sendTcpData
    ///
    /// Send data to the TCP socket
    ///
    /// \param data
    /// \param len
    ///
    virtual void sendTcpData( uint8_t const *data, ssize_t len );

  protected:
    ///
    /// \brief onIncomingTcpHttpData
    ///
    /// Notify the state machine that some data was received
    /// from the APC during the HTTP header section
    ///
    /// \param data ptr to octets
    /// \param len lenth of data in octets
    /// \return length of consumed data
    ///
    virtual ssize_t onIncomingTcpHttpData( uint8_t const *data, ssize_t len );

    ///
    /// \brief onIncomingHttpResponse
    /// \param request
    /// \return
    ///
    virtual bool onIncomingHttpResponse( HttpResponse const &request );

    ///
    /// \brief onIncomingTcpAppData
    ///
    /// Notify the state machine that some data was received
    /// from the APS during the APP message section
    ///
    /// \param data ptr to octets
    /// \param len lenth of data in octets
    /// \return length of consumed data
    ///
    virtual ssize_t onIncomingTcpAppData( uint8_t const *data, ssize_t len );

  protected:
    ///
    /// \brief onAppNop
    ///
    /// Received NOP from APS
    ///
    /// \param msg
    ///
    virtual void onAppNop( AppMessage const &msg );

    ///
    /// \brief onAppEntityIdRequest
    ///
    /// Received ENTITY_ID_REQUEST from APS
    ///
    /// \param msg
    ///
    virtual void onAppEntityIdRequest( AppMessage const &msg );

    ///
    /// \brief onAppEntityIdResponse
    ///
    /// Received ENTITY_ID_RESPONSE from APS
    ///
    /// \param msg
    ///
    virtual void onAppEntityIdResponse( AppMessage const &msg );

    ///
    /// \brief onAppLinkUp
    ///
    /// Received LINK_UP from APS
    ///
    /// \param msg
    ///
    virtual void onAppLinkUp( AppMessage const &msg );

    ///
    /// \brief onAppLinkDown
    ///
    /// Received LINK_DOWN from APS
    ///
    /// \param msg
    ///
    virtual void onAppLinkDown( AppMessage const &msg );

    ///
    /// \brief onAppAvdeccFromAps
    ///
    /// Received AVDECC_FROM_APS from APS
    ///
    /// \param msg
    ///
    virtual void onAppAvdeccFromAps( AppMessage const &msg );

    ///
    /// \brief onAppAvdeccFromApc
    ///
    /// Received AVDECC_FROM_APC from APS
    ///
    /// \param msg
    ///
    virtual void onAppAvdeccFromApc( AppMessage const &msg );

    ///
    /// \brief onAppVendor
    ///
    /// Received VENDOR from APC
    ///
    /// \param msg
    ///
    virtual void onAppVendor( AppMessage const &msg );

    ///
    /// \brief onUnknown
    ///
    /// Received unknown message from APC
    ///
    /// \param msg
    ///
    virtual void onAppUnknown( AppMessage const &msg );

  protected:
    ApcStateMachine *m_owner;

    bool m_in_http;
    HttpClientParser *m_http_parser;
    std::string m_path;
    AppMessageParser m_app_parser;
};

}

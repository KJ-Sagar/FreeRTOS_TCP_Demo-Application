#include "FreeRTOS.h"
#include "task.h"
#include "tcp_heartbeat_demo.h"

void vStartHeartbeatDemo( void )
{
    FreeRTOS_printf( ( "HB: Starting Heartbeat demo\n" ) );

    vStartTCPHeartbeatDemo();
}

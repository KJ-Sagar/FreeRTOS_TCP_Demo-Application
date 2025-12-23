#include "demo_selector.h"
#include "demo_echo.h"
#include "demo_heartbeat.h"
#include "app_config.h"

void vDemoSelectorStart( void )
{
#if ( APP_DEMO_ECHO == 1 )
    vStartEchoClient();

#elif ( APP_DEMO_HEARTBEAT == 1 )
    vStartHeartbeatDemo();

#else
    FreeRTOS_printf( ( "APP: No demo selected\n" ) );
#endif
}

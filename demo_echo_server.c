static void prvEchoServerTask( void * pvParameters )
{
    Socket_t xListenSocket, xClientSocket;
    struct freertos_sockaddr xBindAddress;
    char rxBuffer[128];
    BaseType_t xRx;
    TickType_t xTimeout = pdMS_TO_TICKS( 5000 );

    ( void ) pvParameters;

    FreeRTOS_printf( ( "ECHO SERVER: Starting on port %d\r\n", ECHO_SERVER_PORT ) );

    xListenSocket = FreeRTOS_socket(
        FREERTOS_AF_INET,
        FREERTOS_SOCK_STREAM,
        FREERTOS_IPPROTO_TCP
    );

    configASSERT( xListenSocket != FREERTOS_INVALID_SOCKET );

    memset( &xBindAddress, 0, sizeof( xBindAddress ) );
    xBindAddress.sin_port = FreeRTOS_htons( ECHO_SERVER_PORT );

    FreeRTOS_bind( xListenSocket, &xBindAddress, sizeof( xBindAddress ) );
    FreeRTOS_listen( xListenSocket, 2 );

    for( ;; )
    {
        FreeRTOS_printf( ( "ECHO SERVER: Waiting for client...\r\n" ) );

        xClientSocket = FreeRTOS_accept( xListenSocket, NULL, NULL );
        if( xClientSocket == FREERTOS_INVALID_SOCKET )
            continue;

        FreeRTOS_setsockopt(
            xClientSocket,
            0,
            FREERTOS_SO_RCVTIMEO,
            &xTimeout,
            sizeof( xTimeout )
        );

        FreeRTOS_printf( ( "ECHO SERVER: Client connected\r\n" ) );

        for( ;; )
        {
            xRx = FreeRTOS_recv(
                xClientSocket,
                rxBuffer,
                sizeof( rxBuffer ) - 1,
                0
            );

            if( xRx <= 0 )
                break;

            rxBuffer[xRx] = '\0';
            FreeRTOS_printf( ( "ECHO SERVER RX: %s\r\n", rxBuffer ) );

            FreeRTOS_send(
                xClientSocket,
                rxBuffer,
                xRx,
                0
            );
        }

        FreeRTOS_printf( ( "ECHO SERVER: Client disconnected\r\n" ) );
        FreeRTOS_closesocket( xClientSocket );
    }
}

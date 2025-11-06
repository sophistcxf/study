const WebSocket = require('ws');
const net = require('net');

const wss = new WebSocket.Server({ port: 3000 });

wss.on('connection', (ws) => {
    let tcpServer;
    let port;

    ws.on('message', (message) => {
        ws.send(`Server received: ${message}`);
    });

    ws.on('close', () => {
        if (tcpServer) {
            tcpServer.close();
        }
    });

    // Get port from URL query parameter
    const urlParams = new URLSearchParams(ws.upgradeReq.url.split('?')[1]);
    port = parseInt(urlParams.get('port'));

    if (!port || port < 1 || port > 65535) {
        ws.send('Invalid port number');
        ws.close();
        return;
    }

    // Create TCP server
    tcpServer = net.createServer((socket) => {
        socket.on('data', (data) => {
            ws.send(`TCP Data received on port ${port}: ${data.toString()}`);
        });

        socket.on('end', () => {
            ws.send(`TCP Connection closed on port ${port}`);
        });

        socket.on('error', (err) => {
            ws.send(`TCP Error on port ${port}: ${err.message}`);
        });
    });

    tcpServer.on('error', (err) => {
        ws.send(`TCP Server error on port ${port}: ${err.message}`);
    });

    tcpServer.listen(port, () => {
        ws.send(`TCP Server listening on port ${port}`);
    });
});

console.log('WebSocket server running on ws://localhost:3000');

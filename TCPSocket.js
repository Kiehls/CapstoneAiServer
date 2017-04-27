/**
 * Created by Netapp on 2017-04-27.
 */
// Keep track of the chat clients
var clients = [];

// Start a TCP Server
//net.createServer(function (socket) {
module.exports = function(socket) {
    socket.createServer(function (_socket) {
        // Identify this client
        _socket.name = _socket.remoteAddress + ":" + _socket.remotePort

        // Put this new client in the list
        clients.push(_socket);

        // Send a nice welcome message and announce
        _socket.write("Welcome " + _socket.name + "\n");
        broadcast(_socket.name + " joined the chat\n", _socket);

        // Handle incoming messages from clients.
        _socket.on('data', function (data) {
            broadcast(_socket.name + "> " + data, _socket);
        });

        var nextPos = {
            "B_Pos": 1,
            "A_Pos": 11
        };
        var nextJson = JSON.stringify(nextPos);
        _socket.write("Board Data" + nextJson);

        // Remove the client from the list when it leaves
        _socket.on('end', function () {
            clients.splice(clients.indexOf(_socket), 1);
            broadcast(_socket.name + " left the chat.\n");
        });

        // Send a message to all clients
        function broadcast(message, sender) {
            clients.forEach(function (client) {
                // Don't want to send it to sender
                if (client === sender) return;
                client.write(message);
            });
            // Log it to the server output too
            process.stdout.write(message)
        }
    }).listen(6120);
    console.log("Graph Socket App on port 6120");

    // // Identify this client
    // socket.name = socket.remoteAddress + ":" + socket.remotePort
    //
    // // Put this new client in the list
    // clients.push(socket);
    //
    // // Send a nice welcome message and announce
    // socket.write("Welcome " + socket.name + "\n");
    // broadcast(socket.name + " joined the chat\n", socket);
    //
    // // Handle incoming messages from clients.
    // socket.on('data', function (data) {
    //     broadcast(socket.name + "> " + data, socket);
    // });
    //
    // var nextPos = {
    //     "B_Pos": 1,
    //     "A_Pos": 11
    // };
    // var nextJson = JSON.stringify(nextPos);
    // socket.write("Board Data" + nextJson);
    //
    // // Remove the client from the list when it leaves
    // socket.on('end', function () {
    //     clients.splice(clients.indexOf(socket), 1);
    //     broadcast(socket.name + " left the chat.\n");
    // });
    //
    // // Send a message to all clients
    // function broadcast(message, sender) {
    //     clients.forEach(function (client) {
    //         // Don't want to send it to sender
    //         if (client === sender) return;
    //         client.write(message);
    //     });
    //     // Log it to the server output too
    //     process.stdout.write(message)
    // }

}//).listen(5000);
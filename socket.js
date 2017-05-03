/**
 * Created by Netapp on 2017-04-01.
 */

//io = server , socket = client(Android)
var _Socket_;
var _EngineCount = 1;
var _GraphCount = 0;
// Start a Socket.IO Server
module.exports = function (io, net, exec) {

    //-----------------------------Inter Connection Between Graph Module-------------------
    net.createServer(function (_socket) {
        _Socket_ = _socket;
        // Identify this client
        _socket.name = _socket.remoteAddress + ":" + _socket.remotePort

        // Send a nice welcome message and announce
        //_Socket_.write("Welcome " + _socket.name + "\n", _socket); --> Error
        console.log('==========================Server Output==================================================');
        process.stdout.write(_socket.name + " Graph Module Connected\n", _socket)

        // Handle incoming messages from clients.
        _Socket_.on('data', function (data) {
            console.log('==========================Data From Graph==================================================');
            process.stdout.write(_socket.name + "> " + data + ' | ' + _GraphCount + "\n", _socket);
            _GraphCount++;

            var nextJson = '7879';

            //Send Locations to Unity Client
            io.emit('App response', nextJson);
            console.log("JSON Data Transmit: " + nextJson);
        });

        // Remove the client from the list when it leaves
        _Socket_.on('end', function () {
            process.stdout.write(_socket.name + " Graph Module Disconnected.\n");
        });
    }).listen(6120);
    console.log("Graph Socket App on port 6120");
    //-----------------------------Connection Between Unity Game Engine--------------------
    io.on('connection', function (socket) {
        console.log('==========================Server Output==================================================');
        console.log('Unity Game Engine Connected');

        //Android starts Game(First Connection).
        io.emit('App start', 'Initialize Korean Chess Game');

        //Android send request & server send response
        socket.on('App request', function (pos) {
            console.log('==========================Data From Unity==================================================');
            console.log(pos + " | " + _EngineCount++);

            //Json Object parsing
            var temps = JSON.parse(pos);
            _Socket_.write(temps.Host.toString() + '|' + temps.Board.toString());
        });

        //User Disconnected
        socket.on('App disconnect', function () {
            socket.disconnect();
            console.log('Unity Game Engine Disconnected');
            _Socket_.write("");
        });
    });
};
// java.net on the PC, offline: a URL opens a connection that fails the way Java's does with no network
// (connect / getResponseCode / getInputStream throw IOException), so the game's own handlers and
// finally blocks run as written (ThreadDownloadImage disconnects the connection it opened). Skins, the
// resource index and the login check are servers that no longer answer anyway.
#ifndef NETWORK_H
#define NETWORK_H
class URL;
class URLConnection;
class InputStream;
URLConnection* j2xUrlOpenConnection(URL* u);
InputStream* j2xUrlOpenStream(URL* u);
#endif // NETWORK_H

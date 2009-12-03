/*!
 *  $Id: Services.cpp 225 2009-08-21 13:53:51Z artem.iglikov $
 *
 *  @file
 *  @author Artem Iglikov <artem.iglikov@gmail.com>
 *  
 *  @section LICENSE
 *  
 *  This file is part of Qwit.
 *  
 *  Copyright (C) 2008, 2009 Artem Iglikov
 *  
 *  Qwit is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  Qwit is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with Qwit.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  @section DESCRIPTION
 *  
 *  Services class implementation
 */

#ifndef Services_cpp
#define Services_cpp

#include "QwitHeaders.h"

#include "Services.h"

QMap<QString, QMap<QString, QString> > Services::options;
QMap<QString, QMap<QString, QString> > Services::urlShorteners;

void Services::initialize() {
	QMap<QString, QString> twitterOptions;
	twitterOptions["title"] = "Twitter";
	twitterOptions["apiurl"] = "http://twitter.com";
	twitterOptions["baseurl"] = "http://twitter.com";
	twitterOptions["singlemessageurl"] = "http://twitter.com/%username/statuses/%messageid";
	twitterOptions["searchbaseurl"] = "http://search.twitter.com/search?q=";
	twitterOptions["friends"] = "/statuses/friends_timeline";
	twitterOptions["public"] = "/statuses/public_timeline";
	twitterOptions["replies"] = "/statuses/mentions";
	twitterOptions["favorites"] = "/favorites";
	twitterOptions["favor"] = "/favorites/create/";
	twitterOptions["unfavor"] = "/favorites/destroy/";
	twitterOptions["user"] = "/statuses/user_timeline/";
	twitterOptions["last"] = "/users/show/";
	twitterOptions["update"] = "/statuses/update";
	twitterOptions["destroy"] = "/statuses/destroy/";
	twitterOptions["inbox"] = "/direct_messages";
	twitterOptions["outbox"] = "/direct_messages/sent";
	twitterOptions["send"] = "/direct_messages/new";
	twitterOptions["destroydirectmessage"] = "/direct_messages/destroy/";
	twitterOptions["searchapiurl"] = "http://search.twitter.com";
	twitterOptions["search"] = "/search";

	twitterOptions["showFriendships"] = "/statuses/friends";
	twitterOptions["showFollowers"] = "/statuses/followers";
	twitterOptions["showBlocks"] = "/blocks/blocking";
	twitterOptions["createFriendship"] = "/friendships/create";
	twitterOptions["destroyFriendship"] = "/friendships/destroy";
	twitterOptions["createBlock"] = "/blocks/create/";
	twitterOptions["destroyBlock"] = "/blocks/destroy/";

	options["twitter"] = twitterOptions;

	QMap<QString, QString> identicaOptions;
	identicaOptions["title"] = "Identi.ca";
	identicaOptions["apiurl"] = "http://identi.ca/api";
	identicaOptions["baseurl"] = "http://identi.ca";
	identicaOptions["singlemessageurl"] = "http://identi.ca/notice/%messageid";
	identicaOptions["searchbaseurl"] = "http://identi.ca/search/notice?q=";
	identicaOptions["friends"] = "/statuses/friends_timeline";
	identicaOptions["public"] = "/statuses/public_timeline";
	identicaOptions["replies"] = "/statuses/mentions";
	identicaOptions["favorites"] = "/favorites";
	identicaOptions["favor"] = "/favorites/create/";
	identicaOptions["unfavor"] = "/favorites/destroy/";
	identicaOptions["user"] = "/statuses/user_timeline/";
	identicaOptions["last"] = "/users/show/";
	identicaOptions["update"] = "/statuses/update";
	identicaOptions["destroy"] = "/statuses/destroy/";
	identicaOptions["inbox"] = "/direct_messages";
	identicaOptions["outbox"] = "/direct_messages/sent";
	identicaOptions["send"] = "/direct_messages/new";
	identicaOptions["destroydirectmessage"] = "/direct_messages/destroy/";
	// FIXME test whether the following options exist
	identicaOptions["showFriendships"] = "/statuses/friends";
	identicaOptions["showFollowers"] = "/statuses/followers";
	identicaOptions["showBlocks"] = "/blocks/blocking";
	identicaOptions["createFriendship"] = "/friendships/create";
	identicaOptions["destroyFriendship"] = "/friendships/destroy";
	identicaOptions["createBlock"] = "/blocks/create/";
	identicaOptions["destroyBlock"] = "/blocks/destroy/";

	options["identica"] = identicaOptions;

	QMap<QString, QString> customOptions;
	customOptions["title"] = "Custom";
	customOptions["friends"] = "/statuses/friends_timeline";
	customOptions["public"] = "/statuses/public_timeline";
	customOptions["replies"] = "/statuses/mentions";
	customOptions["favorites"] = "/favorites";
	customOptions["favor"] = "/favorites/create/";
	customOptions["unfavor"] = "/favorites/destroy/";
	customOptions["user"] = "/statuses/user_timeline/";
	customOptions["last"] = "/users/show/";
	customOptions["update"] = "/statuses/update";
	customOptions["destroy"] = "/statuses/destroy/";
	customOptions["inbox"] = "/direct_messages";
	customOptions["outbox"] = "/direct_messages/sent";
	customOptions["send"] = "/direct_messages/new";
	customOptions["destroydirectmessage"] = "/direct_messages/destroy/";
	options["custom"] = customOptions;
/*
	QMap<QString, QString> fakeShortenerOptions;
	fakeShortenerOptions["title"] = "Do not shorten urls";
	fakeShortenerOptions["apiurl"] = "";
	fakeShortenerOptions["requesttemplate"] = "";
	fakeShortenerOptions["responseregexp"] = "";
	fakeShortenerOptions["useapikey"] = "";
	fakeShortenerOptions["defaultapikey"] = "";
	fakeShortenerOptions["useusername"] = "";
	fakeShortenerOptions["defaultusername"] = "";
	urlShorteners["0"] = fakeShortenerOptions;
*/
	QMap<QString, QString> murlkzOptions;
	murlkzOptions["title"] = "murl.kz";
	murlkzOptions["apiurl"] = "http://api.murl.kz/basic";
	murlkzOptions["requesttemplate"] = "?api_key=%apikey&format=xml&url=%url";
	murlkzOptions["responseregexp"] = "http://murl.kz/\\w+";
	murlkzOptions["useapikey"] = "true";
	murlkzOptions["defaultapikey"] = "26e4ab94e13d99a20771d4d2a73d202e";
	murlkzOptions["useusername"] = "false";
	murlkzOptions["defaultusername"] = "";
	urlShorteners["murlkz"] = murlkzOptions;

	QMap<QString, QString> unuOptions;
	unuOptions["title"] = "u.nu";
	unuOptions["apiurl"] = "http://u.nu/unu-api-simple";
	unuOptions["requesttemplate"] = "?url=%url";
	unuOptions["responseregexp"] = "http://u.nu/\\w+";
	unuOptions["useapikey"] = "false";
	unuOptions["defaultapikey"] = "";
	unuOptions["useusername"] = "false";
	unuOptions["defaultusername"] = "";
	urlShorteners["unu"] = unuOptions;

	QMap<QString, QString> clckruOptions;
	clckruOptions["title"] = "clck.ru";
	clckruOptions["apiurl"] = "http://clck.ru/--";
	clckruOptions["requesttemplate"] = "?url=%url";
	clckruOptions["responseregexp"] = "http://clck.ru/\\w+";
	clckruOptions["useapikey"] = "false";
	clckruOptions["defaultapikey"] = "";
	clckruOptions["useusername"] = "false";
	clckruOptions["defaultusername"] = "";
	urlShorteners["clckru"] = clckruOptions;

	QMap<QString, QString> uiopmeOptions;
	uiopmeOptions["title"] = "uiop.me";
	uiopmeOptions["apiurl"] = "http://uiop.me/api/newlink.php";
	uiopmeOptions["requesttemplate"] = "?u=%url";
	uiopmeOptions["responseregexp"] = "http://uiop.me/\\w+";
	uiopmeOptions["useapikey"] = "false";
	uiopmeOptions["defaultapikey"] = "";
	uiopmeOptions["useusername"] = "false";
	uiopmeOptions["defaultusername"] = "";
	urlShorteners["uiopme"] = uiopmeOptions;

	QMap<QString, QString> bitlyOptions;
	bitlyOptions["title"] = "bit.ly";
	bitlyOptions["apiurl"] = "http://api.bit.ly/shorten";
	bitlyOptions["requesttemplate"] = "?version=2.0.1&longUrl=%url&login=%username&apiKey=%apikey";
	bitlyOptions["responseregexp"] = "http://bit.ly/\\w+";
	bitlyOptions["useapikey"] = "true";
	bitlyOptions["defaultapikey"] = "R_7a53b974ee8283876867f1efb1b9205e";
	bitlyOptions["useusername"] = "true";
	bitlyOptions["defaultusername"] = "qwit";
	urlShorteners["bitly"] = bitlyOptions;

	QMap<QString, QString> budurlOptions;
	budurlOptions["title"] = "budurl.com";
	budurlOptions["apiurl"] = "http://budurl.com/api/v1/budurls/shrink";
	budurlOptions["requesttemplate"] = "?api_key=%apikey&long_url=%url";
	budurlOptions["responseregexp"] = "http://budurl.com/\\w+";
	budurlOptions["useapikey"] = "true";
	budurlOptions["defaultapikey"] = "";
	budurlOptions["useusername"] = "false";
	budurlOptions["defaultusername"] = "";
	urlShorteners["budurlcom"] = budurlOptions;

	QMap<QString, QString> trimOptions;
	trimOptions["title"] = "tr.im";
	trimOptions["apiurl"] = "http://api.tr.im/v1/trim_simple";
	trimOptions["requesttemplate"] = "?url=%url";
	trimOptions["responseregexp"] = "http://tr.im/\\w+";
	trimOptions["useapikey"] = "false";
	trimOptions["defaultapikey"] = "";
	trimOptions["useusername"] = "false";
	trimOptions["defaultusername"] = "";
	urlShorteners["trim"] = trimOptions;

	QMap<QString, QString> jmpOptions;
	jmpOptions["title"] = "j.mp";
	jmpOptions["apiurl"] = "http://api.j.mp/shorten";
	jmpOptions["requesttemplate"] = "?version=2.0.1&longUrl=%url&login=%username&apiKey=%apikey";
	jmpOptions["responseregexp"] = "http://j.mp/\\w+";
	jmpOptions["useapikey"] = "true";
	jmpOptions["defaultapikey"] = "R_7a53b974ee8283876867f1efb1b9205e";
	jmpOptions["useusername"] = "true";
	jmpOptions["defaultusername"] = "qwit";
	urlShorteners["jmp"] = jmpOptions;

	QMap<QString, QString> migremeOptions;
	migremeOptions["title"] = "migre.me";
	migremeOptions["apiurl"] = "http://migre.me/api.txt";
	migremeOptions["requesttemplate"] = "?url=%url";
	migremeOptions["responseregexp"] = "http://migre.me/\\w+";
	migremeOptions["useapikey"] = "false";
	migremeOptions["defaultapikey"] = "";
	migremeOptions["useusername"] = "false";
	migremeOptions["defaultusername"] = "";
	urlShorteners["migreme"] = migremeOptions;
}

#endif

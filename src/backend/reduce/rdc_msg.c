/*-------------------------------------------------------------------------
 *
 * rdc_msg.c
 *	  interface for messages between Reduce and Reduce,
 *	  also for messages between Reduce and Plan node.
 *
 * Copyright (c) 2016-2017, ADB Development Group
 *
 * IDENTIFICATION
 *		src/backend/reduce/rdc_msg.c
 *
 *-------------------------------------------------------------------------
 */
#include "reduce/rdc_comm.h"
#include "reduce/rdc_msg.h"

RdcPortId	MyReduceId = InvalidPortId;

int
rdc_send_startup_rqt(RdcPort *port, RdcPortType type, RdcPortId id)
{
	StringInfoData	buf;

	AssertArg(port);

#ifdef DEBUG_ADB
	elog(LOG,
		 "send startup request to [%s %ld] {%s:%s}",
		 RdcPeerTypeStr(port), RdcPeerID(port),
		 RdcPeerHost(port), RdcPeerPort(port));
#endif

	initStringInfo(&buf);
	rdc_beginmessage(&buf, MSG_START_RQT);
	rdc_sendint(&buf, RDC_VERSION_NUM, sizeof(int));		/* version */
	rdc_sendint(&buf, type, sizeof(type));
	rdc_sendRdcPortID(&buf, id);
	rdc_endmessage(port, &buf);

	return rdc_flush(port);
}

int
rdc_send_startup_rsp(RdcPort *port, RdcPortType type, RdcPortId id)
{
	StringInfoData	buf;

	AssertArg(port);

#ifdef DEBUG_ADB
	elog(LOG,
		 "send startup response to [%s %ld] {%s:%s}",
		 RdcPeerTypeStr(port), RdcPeerID(port),
		 RdcPeerHost(port), RdcPeerPort(port));
#endif

	initStringInfo(&buf);
	rdc_beginmessage(&buf, MSG_START_RSP);
	rdc_sendint(&buf, RDC_VERSION_NUM, sizeof(int));
	rdc_sendint(&buf, type, sizeof(type));
	rdc_sendRdcPortID(&buf, id);
	rdc_endmessage(port, &buf);

	return rdc_flush(port);
}

int
rdc_send_group_rqt(RdcPort *port, RdcMask *rdc_masks, int num)
{
	StringInfoData	buf;
	int				i = 0;
	RdcMask		   *mask;

	AssertArg(port);
	Assert(num > 1);

	initStringInfo(&buf);
	rdc_beginmessage(&buf, MSG_GROUP_RQT);
	rdc_sendint(&buf, num, sizeof(num));
	for (i = 0; i < num; i++)
	{
		mask = &(rdc_masks[i]);
		Assert(mask);
		Assert(mask->rdc_host[0]);
		Assert(mask->rdc_port > 1024 && mask->rdc_port < 65535);
#ifdef DEBUG_ADB
		elog(LOG, "[Reduce %ld] {%s:%d}", mask->rdc_rpid, mask->rdc_host, mask->rdc_port);
#endif
		rdc_sendRdcPortID(&buf, mask->rdc_rpid);
		rdc_sendint(&buf, mask->rdc_port, sizeof(mask->rdc_port));
		rdc_sendstring(&buf, mask->rdc_host);
	}
	rdc_endmessage(port, &buf);

	return rdc_flush(port);
}

int
rdc_send_group_rsp(RdcPort *port)
{
	StringInfoData	buf;

	AssertArg(port);
	initStringInfo(&buf);
	rdc_beginmessage(&buf, MSG_GROUP_RSP);
	rdc_endmessage(port, &buf);

	return rdc_flush(port);
}

int
rdc_recv_startup_rsp(RdcPort *port, RdcPortType expected_type, RdcPortId expected_id)
{
	char		firstchar;
	StringInfo	msg;

	AssertArg(port);

	msg = RdcInBuf(port);

	firstchar = rdc_getmessage(port, 0);
	if (!(firstchar == MSG_START_RSP || firstchar == MSG_ERROR))
	{
		rdc_puterror(port,
					 "expected startup response from server, "
					 "but received %c", firstchar);
		return EOF;
	}

	/* error response */
	if (firstchar == MSG_ERROR)
	{
		const char *errmsg = rdc_getmsgstring(RdcInBuf(port));
		rdc_puterror(port, "%s", errmsg);
		return EOF;
	} else
	{
		RdcPortType	rsp_type = InvalidPortType;
		RdcPortId	rsp_id = InvalidPortId;
		int			rsp_ver;

		rsp_ver = rdc_getmsgint(msg, sizeof(rsp_ver));
		if (rsp_ver != RDC_VERSION_NUM)
		{
			rdc_puterror(port,
						 "expected version '%d' from server, "
						 "but received response version '%d'",
						 RDC_VERSION_NUM,
						 rsp_ver);
			return EOF;
		}

		rsp_type = rdc_getmsgint(RdcInBuf(port), sizeof(rsp_type));
		if (rsp_type != expected_type)
		{
			rdc_puterror(port,
						 "expected port type '%s' from server, "
						 "but received response type '%s'",
						 rdc_type2string(expected_type),
						 rdc_type2string(rsp_type));
			return EOF;
		}

		rsp_id = rdc_getmsgRdcPortID(RdcInBuf(port));
		if (rsp_id != expected_id)
		{
			rdc_puterror(port,
						 "expected port id '%ld' from server, "
						 "but received response id '%ld'",
						 expected_id, rsp_id);
			return EOF;
		}

		rdc_getmsgend(RdcInBuf(port));
	}

	return 0;
}

int
rdc_recv_group_rsp(RdcPort *port)
{
	char		firstchar;

	AssertArg(port);
	firstchar = rdc_getmessage(port, 0);
	if (firstchar != MSG_GROUP_RSP)
	{
		rdc_puterror(port,
					 "expected group response from server, "
					 "but received %c", firstchar);
		return EOF;
	}
	rdc_getmsgend(RdcInBuf(port));

	return 0;
}
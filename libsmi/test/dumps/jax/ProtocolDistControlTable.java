/*
 * This Java file has been generated by smidump 0.4.5. Do not edit!
 * It is intended to be used within a Java AgentX sub-agent environment.
 *
 * $Id: ProtocolDistControlTable.java 1458 2006-05-29 16:21:11Z strauss $
 */

/**
    This class represents a Java AgentX (JAX) implementation of
    the table protocolDistControlTable defined in RMON2-MIB.

    @version 1
    @author  smidump 0.4.5
    @see     AgentXTable
 */

import java.util.Vector;

import jax.AgentXOID;
import jax.AgentXVarBind;
import jax.AgentXResponsePDU;
import jax.AgentXSetPhase;
import jax.AgentXTable;
import jax.AgentXEntry;

public class ProtocolDistControlTable extends AgentXTable
{

    // entry OID
    private final static long[] OID = {1, 3, 6, 1, 2, 1, 16, 12, 1, 1};

    // constructors
    public ProtocolDistControlTable()
    {
        oid = new AgentXOID(OID);

        // register implemented columns
        columns.addElement(new Long(2));
        columns.addElement(new Long(3));
        columns.addElement(new Long(4));
        columns.addElement(new Long(5));
        columns.addElement(new Long(6));
    }

    public ProtocolDistControlTable(boolean shared)
    {
        super(shared);

        oid = new AgentXOID(OID);

        // register implemented columns
        columns.addElement(new Long(2));
        columns.addElement(new Long(3));
        columns.addElement(new Long(4));
        columns.addElement(new Long(5));
        columns.addElement(new Long(6));
    }

    public AgentXVarBind getVarBind(AgentXEntry entry, long column)
    {
        AgentXOID oid = new AgentXOID(getOID(), column, entry.getInstance());

        switch ((int)column) {
        case 2: // protocolDistControlDataSource
        {
            AgentXOID value = ((ProtocolDistControlEntry)entry).get_protocolDistControlDataSource();
            return new AgentXVarBind(oid, AgentXVarBind.OBJECTIDENTIFIER, value);
        }
        case 3: // protocolDistControlDroppedFrames
        {
            long value = ((ProtocolDistControlEntry)entry).get_protocolDistControlDroppedFrames();
            return new AgentXVarBind(oid, AgentXVarBind.COUNTER32, value);
        }
        case 4: // protocolDistControlCreateTime
        {
            long value = ((ProtocolDistControlEntry)entry).get_protocolDistControlCreateTime();
            return new AgentXVarBind(oid, AgentXVarBind.TIMETICKS, value);
        }
        case 5: // protocolDistControlOwner
        {
            byte[] value = ((ProtocolDistControlEntry)entry).get_protocolDistControlOwner();
            return new AgentXVarBind(oid, AgentXVarBind.OCTETSTRING, value);
        }
        case 6: // protocolDistControlStatus
        {
            int value = ((ProtocolDistControlEntry)entry).get_protocolDistControlStatus();
            return new AgentXVarBind(oid, AgentXVarBind.INTEGER, value);
        }
        }

        return null;
    }

    public int setEntry(AgentXSetPhase phase,
                        AgentXEntry entry,
                        long column,
                        AgentXVarBind vb)
    {

        switch ((int)column) {
        case 2: // protocolDistControlDataSource
        {
            if (vb.getType() != AgentXVarBind.OBJECTIDENTIFIER)
                return AgentXResponsePDU.WRONG_TYPE;
            else
                return ((ProtocolDistControlEntry)entry).set_protocolDistControlDataSource(phase, vb.AgentXOIDValue());
        }
        case 5: // protocolDistControlOwner
        {
            if (vb.getType() != AgentXVarBind.OCTETSTRING)
                return AgentXResponsePDU.WRONG_TYPE;
            else
                return ((ProtocolDistControlEntry)entry).set_protocolDistControlOwner(phase, vb.bytesValue());
        }
        case 6: // protocolDistControlStatus
        {
            if (vb.getType() != AgentXVarBind.INTEGER)
                return AgentXResponsePDU.WRONG_TYPE;
            else
                return ((ProtocolDistControlEntry)entry).set_protocolDistControlStatus(phase, vb.intValue());
        }
        }

        return AgentXResponsePDU.NOT_WRITABLE;
    }

}


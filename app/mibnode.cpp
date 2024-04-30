/*
    Copyright (C) 2004-2011 Martin Jolicoeur (snmpb1@gmail.com) 

    This file is part of the SnmpB project 
    (http://sourceforge.net/projects/snmpb)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mibnode.h"
#include <qtextedit.h>

MibNode::MibNode(enum MibType mibtype, SmiNode *node, MibNode * parent, MibNode * sibling)
    : QTreeWidgetItem(parent, sibling)
    , Type(mibtype)
    , Node(node)
{    
    setText(0, node->name); 
    SetPixmap(FoldState::COLLAPSED);
}

MibNode::MibNode(QString label, QTreeWidget* parent)
    : QTreeWidgetItem(parent)
    , Type(MIBNODE_NODE)
    , Node(nullptr)
{
    setText (0, label);
    SetPixmap(FoldState::COLLAPSED);
}

void MibNode::SetPixmap(FoldState fold)
{
    bool isOpened = (fold == FoldState::EXPANDED);
    switch(Type)
    {
    case MIBNODE_SCALAR:
        setIcon(0, QIcon(":/icon/scalar"));
        break;
    case MIBNODE_COLUMN:
        setIcon(0, QIcon( ":/icon/scalar"));
        break;
    case MIBNODE_ROW:
        setIcon(0, isOpened ? QIcon(":/icon/table-entry-open") : QIcon(":/icon/table-entry"));
        break;
    case MIBNODE_TABLE:	    
        setIcon(0, isOpened ? QIcon(":/icon/table-open") : QIcon(":/icon/table"));
        break;
    case MIBNODE_NOTIFICATION:
        setIcon(0, QIcon(":/icon/alert"));
        break;
    case MIBNODE_GROUP:
        setIcon(0, QIcon(":/icon/group" ));
        break;
    case MIBNODE_COMPLIANCE:
        setIcon(0, QIcon(":/icon/conformance"));
        break;
    case MIBNODE_CAPABILITIES:
        setIcon(0, QIcon(":/icon/capabilities"));
        break;
    case MIBNODE_NODE:
    default:
        setIcon(0, isOpened ? QIcon(":/icon/folder-open") : QIcon(":/icon/folder"));
    }
}

const char *MibNode::GetAccess(void)
{
    switch (Node->access)
    {
    case SMI_ACCESS_NOT_ACCESSIBLE:
        return "not-accessible";
    case SMI_ACCESS_NOTIFY:
        return "notify";
    case SMI_ACCESS_READ_ONLY:
        return "read-only";
    case SMI_ACCESS_READ_WRITE:
        if (Node->nodekind == SMI_NODEKIND_COLUMN)
        {
            SmiNode *p = smiGetParentNode(Node);
            if (p && p->create)
                return "read-create";
        }
        return "read-write";
    case SMI_ACCESS_INSTALL:
        return "install";
    case SMI_ACCESS_INSTALL_NOTIFY:
        return "install-notify";
    case SMI_ACCESS_REPORT_ONLY:
        return "report-only";
    case SMI_ACCESS_UNKNOWN:
    case SMI_ACCESS_EVENT_ONLY:
    case SMI_ACCESS_NOT_IMPLEMENTED:
        break;
    }
	
    return "";
}

const char *MibNode::GetStatus(void)
{
    switch (Node->status)
    {
    case SMI_STATUS_CURRENT:
        return "current";
    case SMI_STATUS_DEPRECATED:
        return "<font color=red>deprecated</font>";
    case SMI_STATUS_MANDATORY:
        return "mandatory";
    case SMI_STATUS_OPTIONAL:
        return "optional";
    case SMI_STATUS_OBSOLETE:
        return "<font color=red>obsolete</font>";
    case SMI_STATUS_UNKNOWN:
        break;
    }

    return "";
}

const char *MibNode::GetKindName(void)
{
    switch(Node->nodekind)
    {
    case SMI_NODEKIND_NODE:
        return "Node";
    case SMI_NODEKIND_SCALAR:
        return "Scalar";
    case SMI_NODEKIND_TABLE:
        return "Table";
    case SMI_NODEKIND_ROW:
        return "Row";
    case SMI_NODEKIND_COLUMN:
        return "Column";
    case SMI_NODEKIND_NOTIFICATION:
        return "Notification";
    case SMI_NODEKIND_GROUP:
        return "Group";
    case SMI_NODEKIND_COMPLIANCE:
        return "Compliance";
    case SMI_NODEKIND_CAPABILITIES:
        return "Capabilities";
    case SMI_NODEKIND_UNKNOWN:
    case SMI_NODEKIND_ANY:
    default:
        break;
    }
    
    return "";
}

const char *MibNode::GetSmiTypeName(void)
{
    switch(Node->decl)
    {    
    /* SMIv1/v2 ASN.1 statements and macros */    
    case SMI_DECL_OBJECTTYPE:
        return "OBJECT-TYPE";
    case SMI_DECL_OBJECTIDENTITY:
        return "OBJECT-IDENTITY";
    case SMI_DECL_MODULEIDENTITY:
        return "MODULE-IDENTITY";
    case SMI_DECL_NOTIFICATIONTYPE:
        return "NOTIFICATION-TYPE";
    case SMI_DECL_TRAPTYPE:
        return "TRAP-TYPE";
    case SMI_DECL_OBJECTGROUP:
        return "OBJECT-GROUP";
    case SMI_DECL_NOTIFICATIONGROUP:
        return "NOTIFICATION-GROUP";
    case SMI_DECL_MODULECOMPLIANCE:
        return "MODULE-COMPLIANCE";
    case SMI_DECL_AGENTCAPABILITIES:
        return "AGENT-CAPABILITIES";
    case SMI_DECL_VALUEASSIGNMENT:
        return "OBJECT-IDENTIFIER";
    /* SMIng statements */
    case SMI_DECL_MODULE:
        return "module";
    case SMI_DECL_NODE:
        return "node";
    case SMI_DECL_SCALAR:
        return "scalar";
    case SMI_DECL_TABLE:
        return "table";
    case SMI_DECL_ROW:
        return "row";
    case SMI_DECL_COLUMN:
        return "column";
    case SMI_DECL_NOTIFICATION:
        return "notification";
    case SMI_DECL_GROUP:
        return "group";
    case SMI_DECL_COMPLIANCE:
        return "compliance";
        
    case SMI_DECL_IMPLICIT_TYPE:
    case SMI_DECL_TYPEASSIGNMENT:
    case SMI_DECL_IMPL_SEQUENCEOF:
    case SMI_DECL_TEXTUALCONVENTION:
    case SMI_DECL_MACRO:
    case SMI_DECL_COMPL_GROUP:
    case SMI_DECL_COMPL_OBJECT:
    case SMI_DECL_EXTENSION:
    case SMI_DECL_TYPEDEF:
    case SMI_DECL_UNKNOWN:
    default:
        break;
    }
    
    return "";
}
                                  
const char *MibNode::GetTypeName(void)
{
    SmiType *smiType, *parentType;
    
    smiType = smiGetNodeType(Node);

    if (!smiType || Node->nodekind == SMI_NODEKIND_TABLE)
        return NULL;

    if (smiType->decl == SMI_DECL_IMPLICIT_TYPE)
    {
        parentType = smiGetParentType(smiType);

        if (!parentType)
            return NULL;
        smiType = parentType;
    }

    return (smiType->name);
}

const char *MibNode::GetBaseTypeName(void)
{
    SmiType *smiType;
    
    smiType = smiGetNodeType(Node);

    if (!smiType || Node->nodekind == SMI_NODEKIND_TABLE)
        return "" ;

    switch (smiType->basetype)
    {
    case SMI_BASETYPE_UNSIGNED32:
        return "UNSIGNED32";
    case SMI_BASETYPE_INTEGER32:
        return "INTEGER";
    case SMI_BASETYPE_ENUM:
        return "ENUM";
    case SMI_BASETYPE_OBJECTIDENTIFIER:
        return "OBJECT IDENTIFIER";
    case SMI_BASETYPE_OCTETSTRING:
        return "OCTET STRING";
    case SMI_BASETYPE_BITS:
        return "BITS";
    case SMI_BASETYPE_UNSIGNED64:
        return "UNSIGNED64";
    case SMI_BASETYPE_UNKNOWN:
    default:
        break;
    }

    return "";
}

const char *MibNode::GetOid(void)
{
    // If the Node is NULL, this is the root node, return the iso OID ...
    return (Node?smiRenderOID(Node->oidlen, Node->oid, SMI_RENDER_NUMERIC):"1");
}

// Generate indexes for table rows
QString MibNode::GetRowIndex(SmiNode *smiNode)
{
    SmiNode   *relatedNode;
    SmiNode   *elementNode;
    SmiElement *smiElement;
    QString i;
    int n = 0, j = 0;

    relatedNode = smiGetRelatedNode(smiNode);

    switch (smiNode->indexkind)
    {
    case SMI_INDEX_INDEX:
        i += tr("<tr><td><b>Index(es):</b></td><td>");
        for (smiElement = smiGetFirstElement(smiNode); smiElement != NULL; 
             smiElement = smiGetNextElement(smiElement))
            n++;
        for (smiElement = smiGetFirstElement(smiNode); smiElement != NULL; 
             smiElement = smiGetNextElement(smiElement))
        {
            elementNode = smiGetElementNode(smiElement);
            i += QString("%1").arg(elementNode->name);
            if (smiNode->implied) i += " (Implied)";
            if (++j != n) i += "<br>";
        }
        i += tr("</td></tr>");
        break;
    case SMI_INDEX_AUGMENT:
        if (relatedNode)
            i += tr("<tr><td><b>Augments:</b></td><td>%1</td></tr>")
                         .arg(relatedNode->name);
        break;
    case SMI_INDEX_SPARSE:
        if (relatedNode)
            i += tr("<tr><td><b>Sparse:</b></td><td>%1</td></tr>")
                         .arg(relatedNode->name);
        break;
    case SMI_INDEX_EXPAND:
        if (relatedNode)
            i += tr("<tr><td><b>Expands:</b></td><td>%1</td></tr>")
                         .arg(relatedNode->name);
        break;
    case SMI_INDEX_REORDER:
    case SMI_INDEX_UNKNOWN:
        break;
    }

    return i;
}

QString MibNode::GetSizeRange(void)
{
    SmiType *type = smiGetNodeType(Node);
    QString i;
    SmiRange *r;

    if (type && smiGetFirstRange(type))
    {
        i += tr("<tr><td><b>Size</b></td><td>");
        for (r = smiGetFirstRange(type); r; r = smiGetNextRange(r))
        {
            i += tr("%1 .. %2")
                    .arg(r->minValue.value.unsigned64)
                    .arg(r->maxValue.value.unsigned64);
            if (smiGetNextRange(r))
                i += "<br>";
        }
        i += "</td></tr>";
    }

    return i;
}

QString MibNode::GetValueList(void)
{
    SmiType *type = smiGetNodeType(Node);
    QString i;
    SmiNamedNumber  *nn;

    if (type && smiGetFirstNamedNumber(type))
    {
        i += tr("<tr><td><b>Value List</b></td><td><font color=green>");
        for (nn = smiGetFirstNamedNumber(type); nn; nn = smiGetNextNamedNumber(nn))
        {
            i += QString("%1 (%2)").arg(nn->name).arg(nn->value.value.unsigned32);
            if (smiGetNextNamedNumber(nn))
                i += "<br>";
        }
        i += tr("</font></td></tr>");
    }
    return i;
}

void MibNode::PrintProperties(QString& text)
{
    if (!Node)
        return;

    // Create a table and add elements ...
    text = tr("<table border=\"1\" cellpadding=\"0\" cellspacing=\"0\" align=\"left\">");

    // Add the name
    text += tr("<tr><td><b>Name:</b></td><td><font color=#009000><b>%1</b></font></td>").arg(Node->name);

    // Add the full Oid
    text += tr("<tr><td><b>Oid:</b></td><td>%1</td></tr>")
            .arg(smiRenderOID(Node->oidlen, Node->oid, SMI_RENDER_NUMERIC));

    // Add misc attributes
    text += tr("<tr><td><b>Composed Type:</b></td><td>%1</td></tr>").arg(GetTypeName());
    text += tr("<tr><td><b>Base Type:</b></td><td>%1</td></tr>").arg(GetBaseTypeName());
    text += tr("<tr><td><b>Status:</b></td><td>%1</td></tr>").arg(GetStatus());
    text += tr("<tr><td><b>Access:</b></td><td>%1</td></tr>").arg(GetAccess());
    text += tr("<tr><td><b>Kind:</b></td><td>%1</td></tr>").arg(GetKindName());
    if (Node->nodekind == SMI_NODEKIND_ROW)
        text += GetRowIndex(Node);
    text += tr("<tr><td><b>SMI Type:</b></td><td>%1</td></tr>").arg(GetSmiTypeName());

    // Add size range
    text += GetSizeRange();

    // Add units (seconds, bits, ....)
    if (Node->units)
        text += tr("<tr><td><b>Units:</b></td><td>%1</td></tr>").arg(Node->units);

    // Add value list 
    text += GetValueList();

    // Add module
    text += tr("<tr><td><b>Module:</b></td><td>%1</td></tr>")
            .arg(smiGetNodeModule(Node)->name);

    // Add the reference
    if (Node->reference)
    {
        text += tr("<tr><td><b>Reference:</b></td><td><font face=fixed color=blue>");
        text += Qt::convertFromPlainText (Node->reference);
        text += tr("</font></td></tr>");
    }

    // Add the description
    text += tr("<tr><td><b>Description:</b></td><td><font face=fixed color=blue>");
    text += Qt::convertFromPlainText (Node->description);
    text += tr("</font></td></tr>");

    text += tr("</table>");
}


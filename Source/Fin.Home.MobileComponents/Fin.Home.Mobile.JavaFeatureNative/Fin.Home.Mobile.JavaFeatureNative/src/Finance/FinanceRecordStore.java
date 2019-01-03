/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Finance;

import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.*;
import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.io.file.FileSystemRegistry;
import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.*;

public class FinanceRecordStore
{
    public static MIDlet FinanceMidlet;
    
    static final int DATE_INDEX = 0;
    static final int ITEM_INDEX = 1;
    static final int AMOUNT_INDEX = 2;
    static final int ISDEBIT_INDEX = 3;
    static final int TIMESTAMP_INDEX = 4;
    static final int CATEGORY_INDEX = 5;
    static final int ISONETIME_INDEX = 6;
    static final int ISBOOKMARK_INDEX = 7;
    static final int FIELD_COUNT = 8;
    
    public static final char FIELD_SEPERATOR = '\t';
    
    static final String REC_STORE = "Finance@Mobile@RMS";
    
    static public final String NEW_lINE = System.getProperty("line.separator");
    
    public static Vector GetBlankFinanceRecord()
    {
        Vector rec = new Vector(FIELD_COUNT);
        for(int i = 0; i < FIELD_COUNT; ++i) rec.addElement("");
        return rec;
    }
    
    public static boolean SetFieldInFinanceRecord(int fieldIndex,String value,Vector record)
    {
        if(fieldIndex < 0 || fieldIndex >= FIELD_COUNT || record == null || record.capacity() < FIELD_COUNT) return false;
        record.setElementAt(value, fieldIndex);
        return true;
    }
    
    public static String PackFinanceRecord(Vector record)
    {
        if(record == null || record.capacity() < FIELD_COUNT) return "";
        String recStr = "";
        for(int i = 0; i < FIELD_COUNT; ++i) recStr += record.elementAt(i).toString() + FIELD_SEPERATOR;
        return recStr;
    }
    
    
    public static RecordStore OpenFinanceStore()
    {         
        try
        {
            RecordStore rs = RecordStore.openRecordStore(REC_STORE, true );
            return rs;
        }
        catch (Exception e)
        {
            return null;
        }
    } 
    
    public static boolean RemoveFinanceStore()
    {         
        try
        {
            RecordStore.deleteRecordStore(REC_STORE);
            return true;
        }
        catch (Exception e)
        {
            return false;
        }
    } 
    
    public static boolean AddFinanceRecord(String recStr)
    {
        try
        {
            RecordStore rs = OpenFinanceStore();
            if(rs == null) return false;
            byte[] rec = recStr.getBytes();
            rs.addRecord(rec, 0, rec.length);
            rs.closeRecordStore();
            return true;
        }
        catch (Exception e)
        {
            return false;
        }
    }    
    
    public static String GetField(int fieldIndex, String record)
    {
        if(fieldIndex < 0 || fieldIndex >= FIELD_COUNT || record == null || record.compareTo("") == 0) return "";
        
        try
        {
            int startIdx = -1, endIdx = -1;

            for(int i = 0; i <= fieldIndex; ++i)
            {
                startIdx = endIdx;            
                endIdx = record.indexOf('\t',startIdx + 1); 
            }

            String field = record.substring(startIdx + 1,endIdx);
            return field;
        }
        catch(Exception eX) { return ""; }
    }
    
    public static boolean DeleteFinanceRecord(FinanceRecord record)
    {
        RecordStore rs = null;
        try
        {
            rs = OpenFinanceStore();
            if(rs == null) return true;
            
            byte[] recData = new byte[5]; 
            int len;
            RecordEnumeration renum = rs.enumerateRecords(null, null, false);
            while (renum.hasNextElement())
            {
                int index = renum.nextRecordId();
                int recSize = rs.getRecordSize(index);
                if(recSize <= 0) continue;
                if(recSize > recData.length)
                {
                    recData = new byte[recSize];
                }
                len = rs.getRecord(index, recData, 0);
                String recStr = new String(recData, 0, len);
                
                String recTimeStamp = GetField(TIMESTAMP_INDEX,recStr);
                if(recTimeStamp == "") continue;
                long lngStamp = Long.parseLong(recTimeStamp);
                if(record.RecordID.longValue() == lngStamp)
                {
                    rs.deleteRecord(index);
                    rs.closeRecordStore();
                    rs = null;
                    return true;
                }
            }
            
            if(rs != null) rs.closeRecordStore();
            return false;
        }
        catch (Exception e)
        {
            try { if(rs != null) rs.closeRecordStore(); } catch(Exception eX) {}
            return false;
        }
    }  
    
    public static Vector GetFinanceRecords(String recordDate)
    {
        RecordStore rs = null;
        try
        {
            rs = OpenFinanceStore();
            if(rs == null) return null;
            Vector records = new Vector();
            
            byte[] recData = new byte[5]; 
            int len;
            
            RecordEnumeration renum = rs.enumerateRecords(null, null, false);
            while (renum.hasNextElement())
            {
                int index = renum.nextRecordId();
                int recSize = rs.getRecordSize(index);
                if(recSize <= 0) continue;
                if(recSize > recData.length)
                {
                    recData = new byte[recSize];
                }
                len = rs.getRecord(index, recData, 0);
                String recStr = new String(recData, 0, len);
                
                String recDate = GetField(DATE_INDEX,recStr);
                if(recDate.compareTo(recordDate) == 0)
                {
                    String recDesc = GetField(ITEM_INDEX,recStr);
                    String recAmt = GetField(AMOUNT_INDEX,recStr);
                    String recIsDebit = GetField(ISDEBIT_INDEX,recStr).toLowerCase().trim();
                    String recTimeStamp = GetField(TIMESTAMP_INDEX,recStr);
                    
                    long lngStamp = Long.parseLong(recTimeStamp);
                    
                    FinanceRecord finRec = new FinanceRecord();
                    finRec.RecordID = new Long(lngStamp);
                    finRec.RecordDetails =  recDesc + "(Rs:" + recAmt + ") " + (recIsDebit.compareTo("true") == 0 ? "->" : "<-");
                    records.addElement(finRec);                    
                }
            }
            
            rs.closeRecordStore();
            return records;
        }
        catch (Exception e)
        {
            try { if(rs != null) rs.closeRecordStore(); } catch(Exception eX) {}
            return null;
        }
    }   
    
    public static Vector GetFinanceRecordsForExport()
    {
        RecordStore rs = null;
        try
        {
            rs = OpenFinanceStore();
            if(rs == null) return null;
            Vector records = new Vector();
            
            byte[] recData = new byte[5]; 
            int len;
            
            RecordEnumeration renum = rs.enumerateRecords(null, null, false);
            while (renum.hasNextElement())
            {
                int index = renum.nextRecordId();
                int recSize = rs.getRecordSize(index);
                if(recSize <= 0) continue;
                if(recSize > recData.length)
                {
                    recData = new byte[recSize];
                }
                len = rs.getRecord(index, recData, 0);
                String recStr = new String(recData, 0, len);
                records.addElement(recStr);
            }
            
            rs.closeRecordStore();
            return records;
        }
        catch (Exception e)
        {
            try { if(rs != null) rs.closeRecordStore(); } catch(Exception eX) {}
            return null;
        }
    }   
    
    private static String MakeProperRoot(String root)
    {
        if(root == null || root.compareTo("") == 0)
        {
            return root;              
        }
        root = root.trim().toLowerCase();
        if(!root.startsWith("file:///"))
        {
            root = "file:///" + root;
        }
        if(!root.endsWith("/"))
        {
            root += "/";
        }
        return root;
    }
    
    public static String ImportFinanceRecords(Vector records)
    {
        if(records == null || records.size() <= 0)
        {
            return "No records";
        }
        
        FileConnection fCon = null;
        OutputStreamWriter os = null;
        
        try
        { 
            //Finance@Home-DEBITS_CREDITS: /Finance@Home/DEBITS_CREDITS.txt
            //FinanceMidlet.getAppProperty("Finance@Home-DEBITS_CREDITS");
            String memoryCardPath = System.getProperty("fileconn.dir.memorycard");
            //String memoryCardPath = "E";
            if(memoryCardPath == null || memoryCardPath.compareTo("") == 0)
            {
                memoryCardPath = "file:///E";                
            }
            
            memoryCardPath = MakeProperRoot(memoryCardPath);
            
            fCon = (FileConnection) Connector.open(memoryCardPath, Connector.READ);
            
            boolean pathExists = fCon.exists();
            fCon.close();
            if(!pathExists)
            {                
                Enumeration roots = FileSystemRegistry.listRoots();
                while (roots.hasMoreElements())
                {
                    memoryCardPath = (String)roots.nextElement();                    
                }
                memoryCardPath = MakeProperRoot(memoryCardPath);
            }
            
            memoryCardPath += "Finance@Mobile";
            //"file:///" + memoryCardPath
            fCon = (FileConnection) Connector.open(memoryCardPath, Connector.READ_WRITE);
            if(!fCon.exists())
            {
                fCon.mkdir();
            }
            fCon.close();
            memoryCardPath += "/DEBITS_CREDITS.txt";
            fCon = (FileConnection) Connector.open(memoryCardPath, Connector.READ_WRITE);
            if (fCon.exists())
            {
                fCon.delete();
            }
            fCon.close();
            fCon = (FileConnection) Connector.open(memoryCardPath, Connector.READ_WRITE);
            fCon.create();
            
            os = new OutputStreamWriter(fCon.openOutputStream()); 
            for(int i = 0 ; i < records.size(); ++i)
            {
                os.write(records.elementAt(i) + "\r\n");
                //System.out.println("fCon Write");
            }
            fCon.setHidden(false); 
            fCon.setReadable(true);
            
            if(null != os)
            {
                os.close();
                os = null;
            }
            if(null != fCon)
            {
                fCon.close();
                fCon = null;
            }
            return "";
        }
        catch (Exception e)
        {
            //System.out.println("Exception-" + e.getMessage());
            try 
            { 
                if(null != os) os.close();
                if(null != fCon) fCon.close();
                
            } 
            catch(Exception eX) {}
            
            return e.getMessage();
        }
    }
    
};

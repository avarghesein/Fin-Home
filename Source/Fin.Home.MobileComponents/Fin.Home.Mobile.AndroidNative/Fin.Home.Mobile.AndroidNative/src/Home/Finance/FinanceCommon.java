/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Home.Finance;

import java.io.File;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import android.content.Context;

/**
 *
 * @author Abraham
 */
public class FinanceCommon
{
	public static Context AppContext;
	
	// Database Name
    public static final String DB_NAME = "FinanceDB@Mobile.sqlite";
    
    public static String GetSDCardFinanceFolder()
    {
        String sdcard = android.os.Environment.getExternalStorageDirectory().getAbsolutePath();

        List<String> extNames = new ArrayList<String>();
        extNames.add("external_sd");
        extNames.add("external");
        extNames.add("extSdCard");
        extNames.add("ext_sd");
        extNames.add("");

        for(String extName : extNames)
        {
                File extCardRoot = new File(sdcard + File.separator + extName);
                if(extCardRoot.exists())
                {
                        return extCardRoot + File.separator + "Finance@Mobile";
                }
        }           
        //String exportFile = sdcard.getAbsolutePath() + File.separator+ "external_sd" + File.separator + "Finance@Mobile";
        //String exportFile = sdcard.getAbsolutePath() + File.separator + "Finance@Mobile";
        return "";
    }
    
    public static boolean IsDatabaseAvailable()
    {
    	File dbFile = new File(FinanceCommon.GetSDCardFinanceFolder() + File.separator
                + DB_NAME);
    	
    	return dbFile.exists();
    }
    
    public static long GetNewTimeStamp()
    {
        Calendar cal = Calendar.getInstance();
        cal.set(Calendar.DAY_OF_MONTH, 01);
        cal.set(Calendar.MONTH, Calendar.JANUARY);
        cal.set(Calendar.YEAR, 2000);
        cal.set(Calendar.HOUR, 00);
        cal.set(Calendar.MINUTE, 00);
        cal.set(Calendar.SECOND, 00);
        cal.set(Calendar.AM_PM, Calendar.AM);            
        Date dte2000 = cal.getTime();
        Date dtNow = new Date();            
        return ( (dtNow.getTime() - dte2000.getTime()) / 1000);
    }       

    public static String GetDoubleAsString(double value)
    {
        long lngVal = (long)value;
        if((lngVal - value) == 0) return String.valueOf(lngVal);
        return String.valueOf(value);
    }
};

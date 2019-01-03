package Home.Finance;

import java.text.SimpleDateFormat;
import java.util.Date;

import android.text.format.DateFormat;
import java.util.Calendar;

public class DateUtility
{
	public static Calendar SelectedDate;
        
        public static Calendar SelectedCalendar;
	
	public static CharSequence SelectedDateAsString()
	{
		return FormatDate(SelectedDate, "dd-MMM-yyyy");
	}
	
	public static CharSequence FormatDate(String dateStr, String sourceFormat, CharSequence targetFromat)
	{		
		Calendar date = GetDate(dateStr, sourceFormat);       
        return FormatDate(date, targetFromat);        
	}
	
	public static CharSequence FormatDate(Calendar date, CharSequence targetFromat)
	{
            DateFormat format = new DateFormat();
            CharSequence formatted = format.format(targetFromat,date);        
            return formatted;        
	}
	
	public static Calendar CloneCalendar(Calendar cal)
	{
		Calendar clone = Calendar.getInstance();
		clone.setTime(cal.getTime());
		return clone;
	}
	
	public static Calendar GetDate(String dateStr, String sourceFormat)
	{
		try
		{
			SimpleDateFormat simpleFormat = new SimpleDateFormat(sourceFormat);
			Date date = simpleFormat.parse(dateStr);
			Calendar cal = Calendar.getInstance();
			cal.setTime(date);
			return cal;
		}
		catch(Exception eX)
		{
			String x = eX.getMessage();
			return null;
		}
	}
        
    public static CharSequence GetFileDateString(Calendar dte)
    {
        //Calendar cal = Calendar.getInstance();
        //cal.setTime(dte);            
        //String dteStr = ((int)cal.get(Calendar.MONTH) + (int)1) + "/" + cal.get(Calendar.DAY_OF_MONTH) + "/" + cal.get(Calendar.YEAR);
        //return dteStr;
        return FormatDate(dte, "MM/dd/yyyy");
    }
        
      

};

package Home.Finance;

import java.util.Calendar;
import java.util.Date;

public class FinanceEntity
{
	public long TimeStamp;
	public Calendar ExpenseDate;
	public String Title;
	public double Amount;
	public String CategoryCode;
	public boolean IsDebit;
	public boolean IsOneTime;
	public boolean IsBookMark;
        
    public String toString()
    {
        return Title + (IsDebit ? "" : " (Credit*)");
    }
    
    public FinanceEntity DeepClone()
    {
    	FinanceEntity clone = new FinanceEntity();
    	
    	clone.TimeStamp = this.TimeStamp;
    	clone.ExpenseDate = DateUtility.GetDate((String) DateUtility.FormatDate(this.ExpenseDate, "MM/dd/yyyy"), "MM/dd/yyyy");
    	clone.Title = this.Title;
    	clone.Amount = this.Amount;
    	clone.CategoryCode = this.CategoryCode;
    	clone.IsDebit = this.IsDebit;
    	clone.IsOneTime = this.IsOneTime;
    	clone.IsBookMark = this.IsBookMark;
    	
    	return clone;
    }
    
};

package Home.Finance;

import android.database.sqlite.SQLiteDatabase;
import android.os.Environment;

import java.util.ArrayList;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.HashMap;
import java.util.List;

import android.text.format.DateFormat;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.util.*;

public class FinanceAdapter
{
	private static FinanceAdapter _singletonAdapter;
	
	public static FinanceAdapter GetSingleton()
	{
		return _singletonAdapter != null ? _singletonAdapter : (_singletonAdapter = new FinanceAdapter());
	}	
	
	private Map<CharSequence,MonthlyExpenseSummary> _prefetchedMonths;
	private Map<CharSequence,DailyExpenseSummary> _items;	
	private ArrayList<CategoryEntity> _categoryMasterList;
	private ArrayList<FinanceEntity>  _distinctExpenseItems;
	
	private FinanceAdapter()
	{
		_items = new HashMap<CharSequence, DailyExpenseSummary>();
		_prefetchedMonths = new HashMap<CharSequence, MonthlyExpenseSummary>();
	}
	
	private CharSequence GetKey(Calendar date)
	{
        DateFormat format = new DateFormat();
        CharSequence key = format.format("dd-MM-yyyy", date);
        return key;
	}
	
	private int GetIndexInList(ArrayList<FinanceEntity> list, FinanceEntity item)
	{
        if(list == null || item == null) return -1;

        for(int indx = 0 ; indx < list.size(); ++ indx)
        {
                if(list.get(indx).TimeStamp == item.TimeStamp) return indx;
        }
        return -1;
	}
	
	public boolean RefreshExpenseLookupDataset()
    {
        _distinctExpenseItems = null;
		return SQLiteAdapter.GetSingleton().RefreshExpenseLookupDataset();
    }
	 
	private boolean AdjustDailySummary(boolean isDebit, double amount, DailyExpenseSummary dailySummary)
	{
		dailySummary.DailyDebits += isDebit ? amount : 0;
		dailySummary.DailyCredits += !isDebit ? amount : 0;
		return true;
	}
	
	private boolean AdjustMonthlySummary(boolean isDebit, double amount, Calendar date)
	{
		Calendar startDate = new GregorianCalendar(date.get(Calendar.YEAR), date.get(Calendar.MONTH), 01);
		CharSequence key = GetKey(startDate);
		MonthlyExpenseSummary monthSummary = _prefetchedMonths.get(key);
		if(monthSummary != null)
		{
			monthSummary.MonthlyDebits += isDebit ? amount : 0;
			monthSummary.MonthlyCredits += !isDebit ? amount : 0;
			return true;
		}
		return false;
	}
	
	public boolean Prefetch(int month, int year)
	{
            synchronized(this)
            {
		for(int idx = -1; idx <= 1; ++idx)
		{
			Calendar cal = new GregorianCalendar(year, month, 1, 0, 0, 0);
			cal.add(Calendar.MONTH, idx);
			 
			Calendar startDate = DateUtility.CloneCalendar(cal);
			cal.add(Calendar.DATE, cal.getActualMaximum(Calendar.DAY_OF_MONTH) - 1);
			
			Calendar endDate = DateUtility.CloneCalendar(cal);
			
			Prefetch(startDate, endDate);
		}
            }
            return true;
	}	
	
	private boolean Prefetch(Calendar startDate, Calendar endDate)
	{
		//_items.clear();
		CharSequence key = GetKey(startDate);
		if(_prefetchedMonths.containsKey(key)) return true;
		
		MonthlyExpenseSummary monthSummary = new MonthlyExpenseSummary();
		monthSummary.StartDate = startDate;
		monthSummary.EndDate = endDate;
		monthSummary.MonthlyDebits = 0;
		monthSummary.MonthlyCredits = 0;
		_prefetchedMonths.put(key, monthSummary);
		
		ArrayList<FinanceEntity> monthlyItems = SQLiteAdapter.GetSingleton().GetExpenses(startDate, endDate);
		
		if(monthlyItems != null && monthlyItems.size() > 0)
		{
			for(FinanceEntity item : monthlyItems)
			{
				Add(item, false);
			}
		}
		
		return true;
	}
	
	public ArrayList<FinanceEntity> GetList(Calendar date)
	{
		DailyExpenseSummary dailyExp = _items.get(GetKey(date));
		if(dailyExp == null) return null;
		return dailyExp.DailyItems;
	}
	
	public double GetAmoutPerDate(Calendar date, boolean isDebit)
	{
		double sum = 0;
		CharSequence key = GetKey(date);
		if(_items.containsKey(key))
		{
			DailyExpenseSummary dailyExp = _items.get(key);
			sum += isDebit ? dailyExp.DailyDebits : dailyExp.DailyCredits;
		}		
		return sum;
	}
	
	public double GetAmoutPerMonth(int month, int year, boolean isDebit)
	{
		double sum = 0;
		Calendar startDate = new GregorianCalendar(year, month, 01);
		CharSequence key = GetKey(startDate);
		if(_prefetchedMonths.containsKey(key))
		{
			MonthlyExpenseSummary monthSummary = _prefetchedMonths.get(key);
			sum += isDebit ? monthSummary.MonthlyDebits : monthSummary.MonthlyCredits;
		}			
		return sum;
	}
	
	private boolean Add(FinanceEntity item, boolean addToDB)
	{
            synchronized(this)
            {
		CharSequence key = GetKey(item.ExpenseDate);
		if(!_items.containsKey(key))
		{
			DailyExpenseSummary dailyExp = new DailyExpenseSummary();
			dailyExp.DailyCredits = 0;
			dailyExp.DailyDebits = 0;
			_items.put(key, dailyExp);
		}
		
		DailyExpenseSummary dailyExp = _items.get(key);
		
		try
		{
			if(addToDB)
			{
				item.TimeStamp = FinanceCommon.GetNewTimeStamp();
				SQLiteAdapter.GetSingleton().AddExpense(item);
			}
			dailyExp.DailyItems.add(item);	
			AdjustDailySummary(item.IsDebit, item.Amount, dailyExp);
			AdjustMonthlySummary(item.IsDebit, item.Amount,item.ExpenseDate);
            return true;
		}
		catch(Exception eX) {}
            }
            return false;
	}
	
	
	public boolean Add(FinanceEntity item)
	{
		return Add(item, true);
	}
	
	public boolean Edit(FinanceEntity item)
	{
            synchronized(this)
            {
		CharSequence key = GetKey(item.ExpenseDate);
		if(!_items.containsKey(key))
		{
			return false;
		}
		DailyExpenseSummary dailyExp = _items.get(key);
		int idx = GetIndexInList(dailyExp.DailyItems, item);
		if(idx >= 0)
		{
			SQLiteAdapter.GetSingleton().UpdateExpense(item);
			
			FinanceEntity editItem = dailyExp.DailyItems.get(idx);
			
			AdjustDailySummary(editItem.IsDebit, -editItem.Amount, dailyExp);
			AdjustMonthlySummary(editItem.IsDebit, -editItem.Amount,editItem.ExpenseDate);
			
			editItem.Title = item.Title;
			editItem.Amount = item.Amount;
			editItem.CategoryCode = item.CategoryCode;
			editItem.IsDebit = item.IsDebit;
			editItem.IsBookMark = item.IsBookMark;
			editItem.IsOneTime = item.IsOneTime;
			
			AdjustDailySummary(editItem.IsDebit, editItem.Amount, dailyExp);
			AdjustMonthlySummary(editItem.IsDebit, editItem.Amount,editItem.ExpenseDate);
			return true;
		}
            }
            return false;
	}
	
	public boolean Delete(ArrayList<FinanceEntity> items)
	{
            synchronized(this)
            {
		if(items == null || items.size() <= 0) return true;
		
		for(FinanceEntity item : items) Delete(item);
            }
            return true;
	}
	
	private boolean Delete(FinanceEntity item)
	{
		CharSequence key = GetKey(item.ExpenseDate);
		if(!_items.containsKey(key))
		{
			return false;
		}
		DailyExpenseSummary dailyExp = _items.get(key);
		int idx = GetIndexInList(dailyExp.DailyItems, item);
		if(idx >= 0)
		{
			FinanceEntity deleteItem = dailyExp.DailyItems.get(idx);
			SQLiteAdapter.GetSingleton().DeleteExpense(deleteItem);
			dailyExp.DailyItems.remove(idx);
			
			AdjustDailySummary(deleteItem.IsDebit, -deleteItem.Amount, dailyExp);
			AdjustMonthlySummary(deleteItem.IsDebit, -deleteItem.Amount,deleteItem.ExpenseDate);
			
			return true;
		}
		return false;
	}
	
	 public boolean RemoveAll()
	 {
            synchronized(this)
            {
	 	SQLiteAdapter.GetSingleton().DeleteAll();
	     _items.clear();
	     _prefetchedMonths.clear();
            }
	     return true;
	 }
	 
	 public int GetCategoryIndex(String categoryCode)
	 {
	     ArrayList<CategoryEntity> cats = GetCategories();
	     for(int idx = 0; idx < cats.size(); ++idx)
	         if(categoryCode != null && cats.get(idx).Code.compareToIgnoreCase(categoryCode) == 0)
	             return idx;
	     
	     return -1;
	 }
	 
	 public ArrayList<FinanceEntity> GetAutoCompleteList()
	 {
            synchronized(this)
            {
	 	if(_distinctExpenseItems == null)
	 	{
	 		_distinctExpenseItems = SQLiteAdapter.GetSingleton().GetDistinctExpenses();
	 	}
            }
	    return _distinctExpenseItems;
	 }
	 
	 public ArrayList<CategoryEntity> GetCategories()
	 {
            synchronized(this)
            {
	 	if(_categoryMasterList == null)
	 	{
	 		_categoryMasterList = SQLiteAdapter.GetSingleton().GetAllCategories();
	        _categoryMasterList.add(0, new CategoryEntity() {{ Code = ""; Text="<Select>";}});
	 	}
            }
	     return _categoryMasterList;
	 }        
    
//   static final int DATE_INDEX = 0;
//   static final int ITEM_INDEX = 1;
//   static final int AMOUNT_INDEX = 2;
//   static final int ISDEBIT_INDEX = 3;
//   static final int TIMESTAMP_INDEX = 4;
//   static final int CATEGORY_INDEX = 5;
//   static final int ISONETIME_INDEX = 6;
//   static final int ISBOOKMARK_INDEX = 7;
//   static final int FIELD_COUNT = 8;

   public static final char FIELD_SEPERATOR = '\t';
   static public final String NEW_lINE = System.getProperty("line.separator");
   
   private static String CreateFileRecord(FinanceEntity record)
   {
       StringBuilder builder = new StringBuilder();
       
       builder.append(DateUtility.GetFileDateString(record.ExpenseDate));
       builder.append(FIELD_SEPERATOR);
       builder.append(record.Title);
       builder.append(FIELD_SEPERATOR);
       builder.append(FinanceCommon.GetDoubleAsString(record.Amount));           
       builder.append(FIELD_SEPERATOR);
       builder.append(record.IsDebit ? "true" : "false");
       builder.append(FIELD_SEPERATOR);
       builder.append(record.TimeStamp);
       builder.append(FIELD_SEPERATOR);
       builder.append(record.CategoryCode);
       builder.append(FIELD_SEPERATOR);
       builder.append(record.IsOneTime ? "true" : "false");
       builder.append(FIELD_SEPERATOR);
       builder.append(record.IsBookMark ? "true" : "false");
       builder.append(FIELD_SEPERATOR);
       builder.append("\r\n");
       
       return builder.toString();
   }        
   
   private ArrayList<FinanceEntity> GetAll()
   {           
       return SQLiteAdapter.GetSingleton().GetExpenses(null,null);
   }
   
   public CharSequence ExportAll()
   {
       ArrayList<FinanceEntity> records = GetAll();
       
       if(records == null || records.size() <= 0)
       {
           return "No records";
       }
       
       OutputStreamWriter os = null;
       
       try
       {
           File dir = new File(FinanceCommon.GetSDCardFinanceFolder());
           dir.mkdirs();
           File file = new File(dir,"DEBITS_CREDITS.txt");
           FileOutputStream fs = new FileOutputStream(file);
           os = new OutputStreamWriter(fs); 
           for(FinanceEntity record : records)
           {
               String fileRecord = CreateFileRecord(record);
               os.write(fileRecord);
           }

           if(null != os)
           {
               os.close();
               os = null;
           }
           
           if(null != fs)
           {
               fs.close();
               fs = null;
           }
           
           return "";
       }
       catch(Exception e)
       {
           //System.out.println("Exception-" + e.getMessage());
           try 
           { 
               if(null != os) os.close();
           } 
           catch(Exception eX) {}

           return e.getMessage();
       }            
   }
   
	public FinanceEntity SelectedEntity;
	
};

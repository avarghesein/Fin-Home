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
 







import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
 
public class SQLiteAdapter extends SQLiteOpenHelper
{
	
	private static SQLiteAdapter _singletonAdapter;
	
	public static SQLiteAdapter GetSingleton()
	{
		return _singletonAdapter != null ? _singletonAdapter : (_singletonAdapter = new SQLiteAdapter(FinanceCommon.AppContext));
	}
 
    // All Static variables
    // Database Version
    private static final int DB_VERSION = 1;
 
    
 
    // Contacts table name
    private static final String TABLE_EXPENSES = "DEBITS_CREDITS";
    private static final String TABLE_EXPENSES_LOOKUP = "DEBITS_CREDITS_LOOKUP";
    private static final String TABLE_CATEGORY_MASTER = "CATEGORY_MASTER";
 
    // Contacts Table Columns names
    private static final String KEY_Date = "DATE";
    private static final String KEY_Title = "TRANS_DESC";
    private static final String KEY_Amount = "AMOUNT";
    private static final String KEY_Category = "CATEGORY_CODE";
    private static final String KEY_CategoryCode = "CAT_CODE";
    private static final String KEY_CategoryName = "CAT_NAME";
    private static final String KEY_TimeStamp = "TIME_STAMP";
    private static final String KEY_IsDebit = "IS_DEBIT";
    private static final String KEY_IsBookmark = "IS_BOOKMARK";
    private static final String KEY_IsOneTime = "IS_ONETIME";
 
    private SQLiteAdapter(Context context) {
        super(context, FinanceCommon.GetSDCardFinanceFolder() + File.separator
                + FinanceCommon.DB_NAME, null, DB_VERSION);

    }
 
    // Creating Tables
    @Override
    public void onCreate(SQLiteDatabase db)
    {
        
    }
 
    // Upgrading database
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion)
    {
    }
 
    /**
     * All CRUD(Create, Read, Update, Delete) Operations
     */
 
    // Adding new contact
    void AddExpense(FinanceEntity item) {
        SQLiteDatabase db = this.getWritableDatabase();
 
        ContentValues values = new ContentValues();
        values.put(KEY_TimeStamp, item.TimeStamp);
        values.put(KEY_Date, (String) DateUtility.FormatDate(item.ExpenseDate, "MM/dd/yyyy"));
        values.put(KEY_Title, item.Title);
        values.put(KEY_Amount, item.Amount);
        values.put(KEY_Category, item.CategoryCode);
        values.put(KEY_IsDebit, item.IsDebit);
        values.put(KEY_IsOneTime, item.IsOneTime);
        values.put(KEY_IsBookmark, item.IsBookMark);
        // Inserting Row
        db.insert(TABLE_EXPENSES, null, values);
        db.close(); // Closing database connection
    }    
   
    public int UpdateExpense(FinanceEntity item)
    {
        SQLiteDatabase db = this.getWritableDatabase();
 
        ContentValues values = new ContentValues();
        //values.put(KEY_TimeStamp, item.TimeStamp);
        values.put(KEY_Date, (String) DateUtility.FormatDate(item.ExpenseDate, "MM/dd/yyyy"));
        values.put(KEY_Title, item.Title);
        values.put(KEY_Amount, item.Amount);
        values.put(KEY_Category, item.CategoryCode);
        values.put(KEY_IsDebit, item.IsDebit);
        values.put(KEY_IsOneTime, item.IsOneTime);
        values.put(KEY_IsBookmark, item.IsBookMark);
 
        // updating row
        int uCnt =  db.update(TABLE_EXPENSES, values, KEY_TimeStamp + " = ?",
                new String[] { String.valueOf(item.TimeStamp) });
        
        db.close(); // Closing database connection
        
        return uCnt;
    }
 
    // Deleting single contact
    public void DeleteExpense(FinanceEntity item) {
        SQLiteDatabase db = this.getWritableDatabase();
        db.delete(TABLE_EXPENSES, KEY_TimeStamp + " = ?",
                new String[] { String.valueOf(item.TimeStamp) });
        db.close();
    }
    
    // Getting All Contacts
    public ArrayList<CategoryEntity> GetAllCategories()
    {
    	ArrayList<CategoryEntity> catList = new ArrayList<CategoryEntity>();
    	
    	try
    	{    	
	        SQLiteDatabase db = this.getWritableDatabase();
	        
	        Cursor cursor = db.query(true, TABLE_CATEGORY_MASTER,
	        		new String[]
	        		{
		        		KEY_CategoryCode,
		        		KEY_CategoryName
	        		},
	        		null,
	        		null,
	        		null, null, KEY_CategoryName , null);
	        
	        // looping through all rows and adding to list
	        if (cursor.moveToFirst())
	        {
	            do
	            {
	            	CategoryEntity item = new CategoryEntity();
	            	
	                item.Code = cursor.getString(cursor.getColumnIndexOrThrow(KEY_CategoryCode));
	                item.Text = cursor.getString(cursor.getColumnIndexOrThrow(KEY_CategoryName));
	                
	                catList.add(item);
	            }
	            while (cursor.moveToNext());
	        }
	        
	        db.close(); // Closing database connection
    	}
    	catch(Exception eX)
    	{
    		return catList;
    	}
        // return contact list
        return catList;
    }
    
    // Getting All Contacts
    public ArrayList<FinanceEntity> GetExpenses(Calendar startDate, Calendar endDate)
    {
    	ArrayList<FinanceEntity> expenseList = new ArrayList<FinanceEntity>();
    	
    	try
    	{
	        SQLiteDatabase db = this.getWritableDatabase();
	        
	        String whereCondition = "";
	        String[] whereValues = new String[2];
	        int idx = -1;
	        
	        if(startDate != null)
	        {
	        	whereCondition += KEY_Date + " >= ? ";  
	        	whereValues[++idx] = (String) DateUtility.FormatDate(startDate, "MM/dd/yyyy");
	        }
	        
	        if(endDate != null)
	        {
	        	whereCondition += (idx >= 0 ? " AND " : "") + KEY_Date + " <= ? ";
	        	whereValues[++idx] = (String) DateUtility.FormatDate(endDate, "MM/dd/yyyy");
	        }
	        
	        Cursor cursor = db.query(true, TABLE_EXPENSES,
	        		new String[]
	        		{
		        		KEY_TimeStamp,
		        		KEY_Date,
		        		KEY_Title,
		        		KEY_Amount,
		        		KEY_Category,
		        		KEY_IsDebit,
		        		KEY_IsOneTime,
		        		KEY_IsBookmark
	        		},
	        		whereCondition.equalsIgnoreCase("") ? null : whereCondition,
	        		whereCondition.equalsIgnoreCase("") ? null : whereValues,
	        		null, null, KEY_Date , null);
	        
	        // looping through all rows and adding to list
	        if (cursor.moveToFirst())
	        {
	            do
	            {
	            	FinanceEntity item = new FinanceEntity();
	            	
	                item.TimeStamp = cursor.getLong(cursor.getColumnIndexOrThrow(KEY_TimeStamp));
	                item.ExpenseDate = DateUtility.GetDate(cursor.getString(cursor.getColumnIndexOrThrow(KEY_Date)),"MM/dd/yyyy");
	                item.Title = cursor.getString(cursor.getColumnIndexOrThrow(KEY_Title));
	                item.Amount = cursor.getDouble(cursor.getColumnIndexOrThrow(KEY_Amount));
	                item.CategoryCode = cursor.getString(cursor.getColumnIndexOrThrow(KEY_Category));
	                item.IsDebit = cursor.getInt(cursor.getColumnIndexOrThrow(KEY_IsDebit)) != 0;
	                item.IsOneTime = cursor.getInt(cursor.getColumnIndexOrThrow(KEY_IsOneTime)) != 0;
	                item.IsBookMark = cursor.getInt(cursor.getColumnIndexOrThrow(KEY_IsBookmark)) != 0;
	                // Adding contact to list
	                expenseList.add(item);
	            }
	            while (cursor.moveToNext());
	        }
	        
	        db.close(); // Closing database connection
    	}
    	catch(Exception eX)
    	{
    		return expenseList;
    	}
        // return contact list
        return expenseList;
    }
 
    public boolean RefreshExpenseLookupDataset()
    {
    	String refreshQuery = 
    	"INSERT INTO DEBITS_CREDITS_LOOKUP(TRANS_DESC,IS_DEBIT,IS_ONETIME,IS_BOOKMARK,CATEGORY_CODE) " +
    	"SELECT EXP.TRANS_DESC,EXP.IS_DEBIT,EXP.IS_ONETIME,EXP.IS_BOOKMARK,EXP.CATEGORY_CODE " +
    	"FROM " +
    	"(SELECT DISTINCT TRANS_DESC,IS_DEBIT,IS_ONETIME,IS_BOOKMARK,CATEGORY_CODE FROM DEBITS_CREDITS) EXP " +
    	"LEFT JOIN " +
    	"(SELECT TRANS_DESC,IS_DEBIT,IS_ONETIME,IS_BOOKMARK,CATEGORY_CODE FROM DEBITS_CREDITS_LOOKUP) LKUP " +
    	"ON " +
    	"EXP.TRANS_DESC = LKUP.TRANS_DESC AND " +
    	"EXP.IS_DEBIT = LKUP.IS_DEBIT AND " +
    	"EXP.IS_ONETIME = LKUP.IS_ONETIME AND " +
    	"EXP.IS_BOOKMARK = LKUP.IS_BOOKMARK AND " +
    	"EXP.CATEGORY_CODE = LKUP.CATEGORY_CODE " +
    	"WHERE LKUP.TRANS_DESC IS NULL";
    			
    	SQLiteDatabase db = this.getWritableDatabase();
    	db.execSQL(refreshQuery);
    	db.close();
    	
    	return true;
    }

 // Getting All Contacts
    public ArrayList<FinanceEntity> GetDistinctExpenses()
    {
    	ArrayList<FinanceEntity> expenseList = new ArrayList<FinanceEntity>();
        SQLiteDatabase db = this.getWritableDatabase();
        
        Cursor cursor = db.query(true, TABLE_EXPENSES_LOOKUP,
        		new String[]
        		{
	        		KEY_Title,
	        		KEY_Category,
	        		KEY_IsDebit,
	        		KEY_IsOneTime,
	        		KEY_IsBookmark
        		},
        		null,
        		null,
        		null, null, KEY_Title , null);
        
        // looping through all rows and adding to list
        if (cursor.moveToFirst())
        {
            do
            {
            	FinanceEntity item = new FinanceEntity();
            	
                item.Title = cursor.getString(cursor.getColumnIndexOrThrow(KEY_Title));
                item.CategoryCode = cursor.getString(cursor.getColumnIndexOrThrow(KEY_Category));
                item.IsDebit = cursor.getInt(cursor.getColumnIndexOrThrow(KEY_IsDebit)) != 0;
                item.IsOneTime = cursor.getInt(cursor.getColumnIndexOrThrow(KEY_IsOneTime)) != 0;
                item.IsBookMark = cursor.getInt(cursor.getColumnIndexOrThrow(KEY_IsBookmark)) != 0;
                // Adding contact to list
                expenseList.add(item);
            }
            while (cursor.moveToNext());
        }
 
        db.close(); // Closing database connection
        // return contact list
        return expenseList;
    }
 
 
    public boolean DeleteAll()
    {
    	SQLiteDatabase db = this.getWritableDatabase();
        db.delete(TABLE_EXPENSES,null,null);
        db.close();
        return true;
    }
    
    /*
    public int getContactsCount() {
        String countQuery = "SELECT  * FROM " + TABLE_CONTACTS;
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(countQuery, null);
        cursor.close();
 
        // return count
        return cursor.getCount();
    }*/
 
};
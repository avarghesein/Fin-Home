/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Home.Finance;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.CheckBox;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.Toast;
import android.widget.TableRow.LayoutParams;
import android.widget.TextView;
import android.text.format.DateFormat;
import android.view.Gravity;
import android.widget.*;

/**
 * 
 * @author Abraham
 */
public class DailyActivity extends Activity implements android.content.DialogInterface.OnClickListener
{
        private TableLayout _layout;
        private LayoutParams _lp;
        private TableLayout.LayoutParams _tlp;
	/**
	 * Called when the activity is first created.
	 */
	@Override
	public void onCreate(Bundle icicle)
	{
		super.onCreate(icicle);
		setContentView(R.layout.daily_view);
                
		Intent intent = getIntent();
		//_date = intent.getStringExtra("Date");
		//DateUtility.SelectedDate = DateUtility.GetDate(_date, "dd-MM-yyyy");
                
                _lp = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
                _tlp = new TableLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);	
                _layout = (TableLayout) findViewById(R.id.expenses);
                
		PopulateList();
                
	}

        private void AddTableRow(boolean isCheckbox, String text, Object item)
        {
            TableRow tr = new TableRow(this);
            tr.setLayoutParams(_lp);
            TextView tvLeft = isCheckbox ? new CheckBox(this) : new TextView(this);
            if(!isCheckbox)
            {
                tvLeft.setGravity(Gravity.CENTER_HORIZONTAL);
                tvLeft.setTextColor(Color.BLUE);
            }
            else
            {
                tvLeft.setTextColor(Color.BLACK);
            }
            tvLeft.setTag(item);            
            tvLeft.setLayoutParams(_lp);
            tvLeft.setBackgroundColor(Color.TRANSPARENT);
            tvLeft.setText(text);
            tvLeft.setTag(item);
            tr.addView(tvLeft,_lp);
            _layout.addView(tr, _tlp);                
        }
        
	private void PopulateList()
	{
		TextView curDte = (TextView) findViewById(R.id.lblDate);
		curDte.setText(DateUtility.SelectedDateAsString());

		if(_layout.getChildCount() > 1)
		{
                    _layout.removeViews(1, _layout.getChildCount() - 1);
		}
		
		
		Calendar date = DateUtility.SelectedDate;
		ArrayList<FinanceEntity> dailyItems = FinanceAdapter.GetSingleton()
				.GetList(date);

		if (dailyItems == null)
			return;

		for (FinanceEntity item : dailyItems)
		{
			AddTableRow(true, item.Title + " (" + (item.IsDebit ? "" : "Credit*:") + FinanceCommon.GetDoubleAsString(item.Amount) + ")",item);
		}
			
                AddTableRow(false,"",null);
                
                String val = "";
                double itemCount = FinanceAdapter.GetSingleton().GetAmoutPerDate(date, true);
            
                if (itemCount > 0)
                {
                    val = " Daily Debits: " + FinanceCommon.GetDoubleAsString(itemCount) + " /-";
                    AddTableRow(false,val,null);
                }                
                
                itemCount = FinanceAdapter.GetSingleton().GetAmoutPerDate(date, false);

                if (itemCount > 0)
                {
                    val = " Daily Credits: " + FinanceCommon.GetDoubleAsString(itemCount) + " /-";
                    AddTableRow(false,val,null);
                }
	}
	
	ArrayList<FinanceEntity> GetSelectedItems()
	{
		ArrayList<FinanceEntity> items = new ArrayList<FinanceEntity>();
		
		TableLayout t1 = (TableLayout) findViewById(R.id.expenses);
		for(int idx = 1; idx < t1.getChildCount(); ++ idx)
		{
			TableRow row = (TableRow)t1.getChildAt(idx);
			TextView txt = (TextView)row.getChildAt(0);
                        if(!CheckBox.class.isInstance(txt)) continue;
                        CheckBox chk = (CheckBox)txt;
			if(chk.isChecked())
			{
				items.add((FinanceEntity)chk.getTag());
			}
		}
		return items;
	}
	
	@Override
	public void onResume()
	{
		super.onResume();
		PopulateList();
	}

    private int ADD_EXPENSE = Menu.FIRST + 1;
	private int DEL_EXPENSE = Menu.FIRST + 2;
	private int VW_EXPENSE = Menu.FIRST + 3;
	private int EDT_EXPENSE = Menu.FIRST + 4;
        
	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		menu.add(0, ADD_EXPENSE, 0, "Add");
		menu.add(0, DEL_EXPENSE, 0, "Delete Selected");
		menu.add(0, EDT_EXPENSE, 0, "Edit");
		menu.add(0, VW_EXPENSE, 0, "View");

		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public void onClick(DialogInterface arg0, int arg1)
	{
		if(arg0 == _deleteConfirmDlg)
		{
			ArrayList<FinanceEntity> items = GetSelectedItems();
			boolean status = FinanceAdapter.GetSingleton().Delete(items);
			PopulateList();
	        Toast.makeText(getApplicationContext(), status ? "Deleted" : "Failed to delete items", Toast.LENGTH_SHORT).show();
		}
	}
	
	private AlertDialog _deleteConfirmDlg;
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		int selected = item.getItemId();
		
		ArrayList<FinanceEntity> items = GetSelectedItems();
		
		if (selected == DEL_EXPENSE)
		{
			if(items.size() <= 0)
			{
				new AlertDialog.Builder(this).setTitle("Selection Required").setMessage("Select one or more items and retry").show();
			}
			else
			{
				_deleteConfirmDlg = new AlertDialog.Builder(this).setTitle("Confirm Delete").setMessage("Delete selected expense records?")
                .setPositiveButton("Yes",this)
                .setNegativeButton("No", null)
                .show();			
			}
			return super.onOptionsItemSelected(item);
		}
		
		Intent intent = new Intent(this, Home.Finance.ExpenseActivity.class);
		
		if (selected == ADD_EXPENSE)
		{
			intent.putExtra("Action", "Add");
		}
		else
		{
			if(items.size() != 1)
			{
				new AlertDialog.Builder(this).setTitle("Selection Required").setMessage("Select a single item and retry").show();
				return super.onOptionsItemSelected(item);
			}
				
			FinanceAdapter.GetSingleton().SelectedEntity = items.get(0).DeepClone();
					
			if (selected == EDT_EXPENSE)
			{
				intent.putExtra("Action", "Edit");
			}
			if (selected == VW_EXPENSE)
			{
				intent.putExtra("Action", "View");			
			}
		}
		startActivity(intent);
		
		return super.onOptionsItemSelected(item);
	}	
}

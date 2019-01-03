/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Home.Finance;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.*;

/**
 *
 * @author Abraham
 */
public class ExpenseActivity extends Activity
implements 
        OnClickListener,
        AdapterView.OnItemClickListener,
        TextView.OnEditorActionListener
{
	Intent _intent;
	String _action;
	Button _btnOk;
	Button _btnCancel;
	AutoCompleteTextView _txtTitle;
	EditText _txtAmount;
	Spinner _cboCategory;
	CheckBox _chkDebit;
	CheckBox _chkOneTime;
	CheckBox _chkBookMark;

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        setContentView(R.layout.expense_view); 
        
        _intent = getIntent();
        
        _btnOk = (Button)findViewById(R.id.btnOk);
        _btnOk.setOnClickListener(this);
        _btnCancel = (Button)findViewById(R.id.btnCancel);
        _btnCancel.setOnClickListener(this); 
        
        _action = _intent.getStringExtra("Action");
        if(_action.equalsIgnoreCase("Add")) _btnOk.setText("Add");
        if(_action.equalsIgnoreCase("Edit")) _btnOk.setText("Save");
        _btnOk.setVisibility(!_action.equalsIgnoreCase("View") ? View.VISIBLE : View.INVISIBLE);
        
        _txtTitle = (AutoCompleteTextView)findViewById(R.id.txtTitle);
        
        _txtTitle.setOnEditorActionListener(this);
        _txtTitle.setOnItemClickListener(this);
        _txtTitle.setThreshold(1);
        
        _txtAmount = (EditText)findViewById(R.id.txtAmount);
        _txtAmount.setOnEditorActionListener(this);
        
        _cboCategory = (Spinner)findViewById(R.id.txtCategory);
        
       // ArrayAdapter<CategoryEntity> spinAdapter=new ArrayAdapter<CategoryEntity>
       //(this,android.R.layout.simple_list_item_1,FinanceAdapter.GetSingleton().GetCategories());
        
        ArrayAdapter<CategoryEntity> spinAdapter=new ArrayAdapter<CategoryEntity>
       (this,R.layout.custom_spinview,FinanceAdapter.GetSingleton().GetCategories());
        
        spinAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        _cboCategory.setAdapter(spinAdapter);

        _chkDebit = (CheckBox)findViewById(R.id.chkDebit);
        _chkOneTime = (CheckBox)findViewById(R.id.chkOneTime);
        _chkBookMark = (CheckBox)findViewById(R.id.chkMark);
        
        TextView curDte = (TextView) findViewById(R.id.lblDate);
        curDte.setText(DateUtility.SelectedDateAsString());

        ResetFields();
        
        if(    (_action.equalsIgnoreCase("Edit") ||
                _action.equalsIgnoreCase("View")) &&
                FinanceAdapter.GetSingleton().SelectedEntity != null) 
        {
            DataBind(FinanceAdapter.GetSingleton().SelectedEntity);
        }
    }

    public boolean onEditorAction(TextView arg0, int actionId, KeyEvent event)
    {
         if ((event != null && (event.getKeyCode() == KeyEvent.KEYCODE_ENTER)) || (actionId == EditorInfo.IME_ACTION_DONE))
         {
            if(arg0 == _txtTitle)
            {
               _txtAmount.requestFocus();
               return false;
            }

            if(arg0 == _txtAmount && _action.equalsIgnoreCase("Add") && _cboCategory.getSelectedItemPosition() > 0)
            {
                onClick(_btnOk);
            }
         }
         return false;
    }
    
     @Override
    public void onResume()
    {
        super.onResume();

        CustomArrayAdapterExtension<FinanceEntity> autoCompleteAdapter=new CustomArrayAdapterExtension<FinanceEntity>
        (this,android.R.layout.simple_dropdown_item_1line,FinanceAdapter.GetSingleton().GetAutoCompleteList());

        _txtTitle.setAdapter(autoCompleteAdapter);
    }

     
    public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3)
    {
       FinanceEntity item = ((CustomArrayAdapterExtension<FinanceEntity>)arg0.getAdapter()).getItem(arg2);
       DataBind(item);
       _txtAmount.setText("");
       _txtAmount.requestFocus();
    }
    
    private void DataBind(FinanceEntity item)
    {
        if(item != null) 
        {
            _txtTitle.setText(item.Title);            
            _txtAmount.setText(FinanceCommon.GetDoubleAsString(item.Amount));
            
            int catIdx = FinanceAdapter.GetSingleton().GetCategoryIndex(item.CategoryCode);
            if(catIdx >= 0) _cboCategory.setSelection(catIdx);
            
            _chkDebit.setChecked(item.IsDebit);
            _chkBookMark.setChecked(item.IsBookMark);
            _chkOneTime.setChecked(item.IsOneTime);
        }
    }
    
    private void ResetFields()
    {
    	_txtTitle.setText("");
    	_txtAmount.setText("");
        _chkDebit.setChecked(true);
    	_chkOneTime.setChecked(false);
    	_chkBookMark.setChecked(false);
    	_txtTitle.requestFocus();
        _cboCategory.setSelection(0);
    }
    
	@Override
	public void onClick(View arg0)
	{		
		if(arg0.getId() == R.id.btnOk)
		{
			
			FinanceEntity item = null;
			
			if(_action.equalsIgnoreCase("Add")) item = new FinanceEntity();
	        if(_action.equalsIgnoreCase("Edit")) item = FinanceAdapter.GetSingleton().SelectedEntity;
	        
			item.ExpenseDate = DateUtility.SelectedDate;
			item.Title = _txtTitle.getText().toString();
			
			if(item.Title.trim().length() == 0)
			{
				Toast.makeText(this, "Title should not be empty", Toast.LENGTH_SHORT).show();
				_txtTitle.requestFocus();
				return;
			}
			
			try
			{
				item.Amount = Double.parseDouble(_txtAmount.getText().toString());
			}
			catch(Exception eX)
			{
				Toast.makeText(this, "Amount should be a valid number", Toast.LENGTH_SHORT).show();
				_txtAmount.requestFocus();
				return;
			}		
			
                        if(_cboCategory.getSelectedItem() == null)
                        {
                            Toast.makeText(this, "Category should not be empty", Toast.LENGTH_SHORT).show();
                            _cboCategory.requestFocus();
                            return;
                        }
                        
			item.CategoryCode = ((CategoryEntity)_cboCategory.getSelectedItem()).Code;
                        
			if(item.CategoryCode.trim().length() == 0)
			{
				Toast.makeText(this, "Category should not be empty", Toast.LENGTH_SHORT).show();
				_cboCategory.requestFocus();
				return;
			}
			
			item.IsDebit = _chkDebit.isChecked();
			item.IsOneTime = _chkOneTime.isChecked();
			item.IsBookMark = _chkBookMark.isChecked();
			
			boolean status = false;
			
			if(_action.equalsIgnoreCase("Add"))
			{
				status = FinanceAdapter.GetSingleton().Add(item);
				Toast.makeText(this, status ? "Added" : "Adding Failed", Toast.LENGTH_SHORT).show();
			}
			
	        if(_action.equalsIgnoreCase("Edit"))
	        {
	        	status = FinanceAdapter.GetSingleton().Edit(item);
	        	Toast.makeText(this, status ? "Saved" : "Save Failed", Toast.LENGTH_SHORT).show();
	        	finish();
	        	return;
	        }
			
			ResetFields();			
		}
		else			
		{
			finish();
		}
				
	}    
}

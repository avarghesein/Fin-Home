/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Finance;

import java.util.Vector;
import javax.microedition.lcdui.*;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.ItemStateListener;
import javax.microedition.lcdui.TextField;


/**
 *
 * @author AVarghese
 */
public class ExpenseEntryForm extends Form implements CommandListener, ItemStateListener
{
    FinanceMIDlet midlet = null;
    
    private Command addCmd, delCmd, bkCmd;
    
    public void commandAction(Command c, Displayable s)
    {
        if (c == bkCmd)
        {
            midlet.ShowExpenseList();
            midlet.GetExpenseListDisp().Update();           
        }
        else if(c == delCmd)
        {
            
        }
        else if(c == addCmd)
        {
            _record = "";
            if(Validate())
            {
                if(FinanceRecordStore.AddFinanceRecord(_record))
                {
                    _item.setString("");
                    _amountField.setString("");
                    midlet.GetExpenseListDisp().Update();
                    midlet.switchDisplayable(new Alert("Record Successfuly Added"), midlet.GetExpenseListDisp());
                    //midlet.ShowExpenseList();                      
                }
                else
                    midlet.getDisplay().setCurrent(new Alert("Record Adding Failed"),this);                        
            }
        }
    }
    
    public void itemStateChanged(Item item)
    {
        if(item ==  _item)
        {
            String val = _item.getString();
            if( val != null && val.compareTo("") != 0)
            {
                val = " " + val;
                StringBuffer buff = new StringBuffer(val);
                int len = buff.length();
                for(int i = 1; i < len; ++i)
                {
                    if(buff.charAt(i - 1) == ' ' && Character.isLowerCase(buff.charAt(i)))
                    {
                        buff.setCharAt(i, Character.toUpperCase(buff.charAt(i)));
                        ++i;
                    }
                }            
                val = buff.toString().substring(1);
                _item.setString(val);
            }
        }
    }


    TextField _item;
    TextField _amountField;
    ChoiceGroup _categoryField;
    ChoiceGroup _isDebitCredit;
    ChoiceGroup _isOneTime;
    ChoiceGroup _isBookMark;
    String _record;
    
    private boolean Validate()
    {
        Vector finRec = FinanceRecordStore.GetBlankFinanceRecord();
        //0:Date field
        FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.DATE_INDEX, CalendarWidget.GetRecordStoreDateString(midlet.GetExpenseListDisp().SelectedDate), finRec);
        
        String itm = _item.getString().trim();
        if(itm == null || itm.compareTo("") == 0)
        {
            midlet.getDisplay().setCurrent(new Alert("Enter Item"),this);
            return false;
        }
        //1:Item desc field
        FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.ITEM_INDEX, itm, finRec);
        String amt = _amountField.getString().trim();      
       
        float amtFlt = 0;
        
        try
        {
            amtFlt = Float.parseFloat(amt);
        }
        catch(Exception eX)
        {
            midlet.getDisplay().setCurrent(new Alert("Enter a numeric Amount"),this);
            return false;
        }
        //2:Amount field
        FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.AMOUNT_INDEX, "" + amtFlt , finRec);
        
        boolean[] checked = new boolean[] { false };
        
         _isDebitCredit.getSelectedFlags(checked);
         //3:isDebit field
         FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.ISDEBIT_INDEX, (checked[0] ? "true" : "false") , finRec);
         
         //4:=DateDiff("s",CDate("01/01/2000 12:00 AM"),Now()) 
         FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.TIMESTAMP_INDEX, "" + CalendarWidget.GetRecordTimestamp() , finRec);
         
         //5:category field
         FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.CATEGORY_INDEX, CAT_CODES[_categoryField.getSelectedIndex()] , finRec);
         //6:onetime field
         _isOneTime.getSelectedFlags(checked);
         FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.ISONETIME_INDEX, (checked[0] ? "true" : "false") , finRec);
         //7:bookmark field
         _isBookMark.getSelectedFlags(checked);
         FinanceRecordStore.SetFieldInFinanceRecord(FinanceRecordStore.ISBOOKMARK_INDEX, (checked[0] ? "true" : "false") , finRec);
         
         _record = FinanceRecordStore.PackFinanceRecord(finRec);
         if(_record.trim().compareTo("") == 0)
         {
             midlet.getDisplay().setCurrent(new Alert("Invalid details, Try again"),this);
             return false;
         }
         return true;
    }
    
    public ExpenseEntryForm(final FinanceMIDlet m)
    {   
        super("Enter Expense:");
        
        this.midlet = m;      
        
        addCommand(addCmd = new Command("Add", Command.SCREEN, 1));
        addCommand(bkCmd = new Command("Back", Command.SCREEN, 2));        
        setCommandListener(this);
        setItemStateListener(this);
        
        _item = new TextField("Item:*", null, 255, 0);
        append(_item);
        
         _amountField = new TextField("Amount:*", null, 255, TextField.DECIMAL);
        append(_amountField);
        
        _categoryField = new ChoiceGroup ("Category:*", Choice.POPUP ,       
        new String[] {
        "Edible",
        "Vegitable/Fish/Pulse/Fruit",
        "Petrol/Diesel",
        "Home Bills",   
        "Medical",
        "Travel",
        "Stationary",   
        "Vechicle",
        "Cosmetic/Cloth/Personel Wear",            
        "Educational",
        "Electronics",
        "House Hold",                
        "Interest/Tax/Insurance Premium",
        "Personel & Family",
        "Rental",
        "Salary",                     
        "Well Fare/Treat" }, null);
        
        append(_categoryField);
        _isDebitCredit = new ChoiceGroup ("", Choice.MULTIPLE ,
                               new String[] {"Debit?"}, null);
        
        _isDebitCredit.setSelectedIndex(0, true);
        append(_isDebitCredit);
        
        _isOneTime = new ChoiceGroup ("", Choice.MULTIPLE ,
                               new String[] {"Onetime?"}, null);
        append(_isOneTime);
        
        _isBookMark = new ChoiceGroup ("", Choice.MULTIPLE ,
                               new String[] {"Bookmark?"}, null);
        append(_isBookMark);
    }
    
    static final String[] CAT_CODES = new String[]{
                    "EDBL",
                    "VEG",
                    "PET",
                    "EBIL",
                    "MED",
                    "TRV",
                    "STOR",  
                    "ATO",
                    "CLT",                                        
                    "EDU",
                    "ELCT",
                    "HSE",                                        
                    "PRM",
                    "PRS",
                    "RNT",
                    "SAL",                                                          
                    "WFR"
    };
    
}

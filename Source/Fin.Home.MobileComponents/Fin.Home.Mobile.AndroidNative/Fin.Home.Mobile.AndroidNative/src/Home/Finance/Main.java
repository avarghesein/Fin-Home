/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Home.Finance;

import android.app.Application;
import java.util.Calendar;
import java.util.Locale;


public class Main extends Application
{
        @Override public void onCreate()
        {
            super.onCreate();
            //DateUtility.SelectedCalendar = Calendar.getInstance(Locale.getDefault());
        }
}
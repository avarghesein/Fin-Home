package Home.Finance;

import java.util.ArrayList;
import java.util.Map;

public class DailyExpenseSummary
{
	public double DailyDebits;
	public double DailyCredits;
	public ArrayList<FinanceEntity> DailyItems;	
	
	public DailyExpenseSummary()
	{
		DailyItems = new ArrayList<FinanceEntity>();
	}
};

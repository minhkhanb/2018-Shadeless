package topebox.core.Actions;
import topebox.core.*;

public class ActionTapdaqPrepareInterstitial implements Action{
	ActionArg Arg = null;
	public ActionTapdaqPrepareInterstitial(ActionArg arg){
		Arg = arg;
	}
	public void act(){
		if(Arg == null){
			return;
		}
		Arg.onBegin();
		final ActionArg arg = Arg;
		if(TapdaqManager.isPrepareInterstitialCompleted()){
			Arg.onDone();
		}else{
			TapdaqManager.prepareInterstitial(arg);
		}
	}
}
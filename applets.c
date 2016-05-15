#include <string.h>
#include <panel-applet.h>
#include <gtk/gtk.h>

GtkWidget *label,*labelw,*event_box,*window;
GtkTooltips *tooltips;
PanelApplet *applet;
long long i=0,tt=0,tt0=0,idle=0,idle0=0,r1=0,t1=0,r0=0,t0=0,mu=0,mt=0,mf=0;;

char *uptime(char *hms)
{
 int ss,h,m,s; 
 float tt,it;
 FILE *fp;
 fp=fopen("/proc/uptime","r");
 fscanf(fp,"%f %f",&tt,&it);
 fclose(fp);
 ss=(int)tt;
 h=ss/3600;
 m=ss%3600/60;
 s=ss%3600%60;
 sprintf(hms,"%d:%02d:%02d",h,m,s);
 return hms;
}

void meminfo()
{ 
 FILE *fp;
 char cmt[40],cmf[40],a[40],b[40];
 fp=fopen("/proc/meminfo","r"); 
 fgets(cmt,sizeof(cmt),fp);
 fgets(cmf,sizeof(cmf),fp);
 fclose(fp);
 sscanf(cmt,"%s%d%s",a,&mt,b);
 sscanf(cmf,"%s%d%s",a,&mf,b);
 //printf("MEM: %lldB/%lldB\n",mf*1024,mt*1024);
}

char *cpustat(char *cpusage)
{ 
 FILE *fp;
 char ch[100],cpu[10]; 
 fp=fopen("/proc/stat","r"); 
 fgets(ch,sizeof(ch),fp); 
 fclose(fp);
 long user,nice,sys,idle,iowait,irq,softirq,usage=0;
 sscanf(ch,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
 tt=user+nice+sys+idle+iowait+irq+softirq;
 if(i>0)usage=((tt-tt0)-(idle-idle0))*100/(tt-tt0);
 //printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
 sprintf(cpusage,"%d%%",usage);
 idle0=idle;
 tt0=tt;
 i++;
 return cpusage;
}

void netdev()
{
 FILE *fp;
 char ch[150],ch4[150],itf[10];
 int r2,r3,r4,r5,r6,r7,r8,t2,t3,t4,t5,t6,t7,t8;
 fp=fopen("/proc/net/dev","r");
 fgets(ch,sizeof(ch),fp);
 fgets(ch,sizeof(ch),fp);
 fgets(ch,sizeof(ch),fp); //deepin delete this line
 fgets(ch4,sizeof(ch4),fp); 
 fclose(fp);
 //printf("%s\n",ch4);
 sscanf(ch4,"%s%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",itf,&r1,&r2,&r3,&r4,&r5,&r6,&r7,&r8,&t1,&t2,&t3,&t4,&t5,&t6,&t7,&t8);
 //printf("%d,%d\n",r1,t1);
}

char *B2G(long long b)
{
 static char g[10];
 if(b>999999999){
  sprintf(g,"%.2fGB",(float)b/1073741824);
 }else{
  if(b>999999){
   sprintf(g,"%.2fMB",(float)b/1048576);
  }else{
   if(b>999){
    sprintf(g,"%.2fKB",(float)b/1024);
   }else{
    sprintf(g,"%dB",b);
   }
  }
 }
 return g;
}

gint settime(gpointer data)
{ 
 char lt[100],lm[100],ltip[200],cr[10],ct[10],crs[10],cts[10],hms[10],cmu[10],cmt[10],cpusage[20];
 int rs,ts,memusage=0;
 uptime(hms);
 cpustat(cpusage);
 meminfo(); 
 mu=mt-mf;
 memusage=mu*100/mt;
 strcpy(cmt,B2G(mt*1024));
 strcpy(cmu,B2G(mu*1024));
 netdev();
 strcpy(cr,B2G(r1));
 strcpy(ct,B2G(t1));
 rs=r1-r0;
 ts=t1-t0;
 strcpy(crs,B2G(rs));
 strcpy(cts,B2G(ts)); 
 sprintf(lt,"D:%s/s\nU:%s/s",crs,cts);
 sprintf(lm,"<span foreground='white'>D:%s/s\nU:%s/s</span>",crs,cts);
 sprintf(ltip,"开机: %s\nCPU: %s\n内存: %s/%s=%d%%\n下载: %s , %s/s\n上传: %s , %s/s",hms,cpusage,cmu,cmt,memusage,cr,crs,ct,cts);
 g_message(lt);
 //gtk_label_set_text(GTK_LABEL(label),lt);
 gtk_label_set_markup(GTK_LABEL(label), lm);
 gtk_tooltips_set_tip(tooltips,label,ltip,NULL);
 r0=r1;
 t0=t1;
 return TRUE;
}

void about_dialog ()
{
  g_message ("about...");
  GtkWidget *dialog;
  const gchar *authors[] = { "sonichy@163.com", NULL };
  const gchar *documenters[] = { "http://www.mimuw.edu.pl/~jbednarz/AviraTrial/antivir-server-prof-3.1.3.5-2/contrib/applet/avira-guard-applet.c","https://projects.gnome.org/ORBit2/appletstutorial.html", NULL };
  dialog = gtk_about_dialog_new ();
  //gtk_widget_set_size_request(GTK_WIDGET(window),200,100);
  gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (dialog), NULL);
  gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (dialog), "网速监控");
  gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), "1.0");
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog), "Copyright © 2016 黄颖");
  gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), "在任务栏显示网速");
  gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), "http://sonichy.hjwhp.ghco.info");
  gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);
  gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (dialog), documenters);
  gtk_about_dialog_set_translator_credits (GTK_ABOUT_DIALOG (dialog), "黄颖\nsonichy");
  gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (dialog), "GNU GPL");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void properties_dialog()
{}

void draw_start(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
 switch(event->button)
 {
  case 1:
   window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request(GTK_WIDGET(window),220,100);
   gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
   labelw=gtk_label_new("开机:\nCPU:\n内存:\n下载:\n上传:");
   gtk_container_add(GTK_CONTAINER(window),label);
   gtk_widget_show_all(window);
   break;
 }
}

static const char Context_menu_xml [] =
   "<popup name=\"button3\">\n"
   "   <menuitem name=\"Properties Item\" "
   "             verb=\"Properties\" "
   "           _label=\"属性\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-properties\"/>\n"
   "   <menuitem name=\"About Item\" "
   "             verb=\"About\" "
   "           _label=\"关于\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-about\"/>\n"
   "</popup>\n";
   
static const BonoboUIVerb myexample_menu_verbs [] = {
        BONOBO_UI_VERB ("Properties", properties_dialog),
        BONOBO_UI_VERB ("About", about_dialog),
        BONOBO_UI_VERB_END
};

//static void myexample_applet_properties_dialog (BonoboUIComponent *uic, struct MultiRes *applet)
//{
	/* Construct the properties dialog and show it here */

//}

static gboolean myexample_applet_fill (PanelApplet *applet, const gchar *iid, gpointer data)
{

	if (strcmp (iid, "OAFIID:ExampleApplet") != 0)
		return FALSE;
 //event_box = gtk_event_box_new();
 label = gtk_label_new ("D:0B/s\nU:0B/s");
 gtk_container_add (GTK_CONTAINER (applet), label);
 //gtk_container_add (GTK_CONTAINER (event_box), label);
 //gtk_container_add (GTK_CONTAINER (applet), event_box);
 //gtk_signal_connect(GTK_OBJECT(event_box),"button_press_event", GTK_SIGNAL_FUNC(draw_start), "NULL");
 tooltips=gtk_tooltips_new();
 gtk_tooltips_set_tip(tooltips,label,"网速",NULL);
	gtk_widget_show_all (GTK_WIDGET (applet));
 gint s=g_timeout_add(1000,settime,NULL);
 panel_applet_setup_menu (applet,
                         Context_menu_xml,
                         myexample_menu_verbs,
                         NULL);
	return TRUE;
}
PANEL_APPLET_BONOBO_FACTORY ("OAFIID:ExampleApplet_Factory", PANEL_TYPE_APPLET, "The Hello World Applet", "0", myexample_applet_fill, NULL);
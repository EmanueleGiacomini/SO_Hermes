#include "gui.h"


void sendPacket(int fd, PacketHandler* ph, PacketHeader* h, uint8_t* sem) {
  while(!*sem); // Wait if someone is sending
  *sem = 0;
  PacketHandler_sendPacket(ph, h);
  uint8_t bytes = PacketHandler_txSize(ph); 
  for(int i=0; i<bytes; i++) {
    uint8_t c = PacketHandler_writeByte(ph);
    write(fd, &c, 1);
  }
  *sem = 1;
}

MotorParamsPacket mpp = {
  {
    .id=ID_MOTOR_PARAMS_PACKET,
    .size=sizeof(mpp),
    .seq=0,
    .dest_addr=0,
    .src_addr=0,
    .checksum=0,
  },
  .dir_a_pin=0,
  .dir_b_pin=0,
  .pwm_pin=0,
  .kp=0,
  .ki=0,
  .kd=0,
  .max_i=0,
  .sum_i=0,
  .max_output=0,
  .dt=0,
  .idt=0,
};

MotorStatusPacket* _motor_status;
PacketHandler* _ph;
uint8_t* _end_flag;
uint8_t* _send_sem;
int _fd;

//int active = 0;
GtkWidget *et;
GtkWidget *ds;
GtkWidget *ms;
GtkWidget *kp;
GtkWidget *ki;
GtkWidget *kd;
GtkWidget *dt;

void* routine(void *arg) {
  char val[10];

  while(!*_end_flag) {
    sprintf(val, "%d", _motor_status->encoder_ticks);
    gtk_label_set_text(GTK_LABEL (et), val);
    
    sprintf(val, "%d", _motor_status->desired_speed);
    gtk_label_set_text(GTK_LABEL (ds), val);
    
    sprintf(val, "%d", _motor_status->measured_speed);
    gtk_label_set_text(GTK_LABEL (ms), val);
    
    usleep(1000 * 10);
  }

  gtk_main_quit();
  pthread_exit(NULL);
}

static void send_packet(GtkWidget *widget, gpointer data) {
  
  char buf[256];

  double kp_value;
  double ki_value;
  double kd_value;
  double dt_value;

  // Extract values
  kp_value = atof(gtk_entry_get_text (GTK_ENTRY (kp)));
  ki_value = atof(gtk_entry_get_text (GTK_ENTRY (ki)));
  kd_value = atof(gtk_entry_get_text (GTK_ENTRY (kd)));
  dt_value = atof(gtk_entry_get_text (GTK_ENTRY (dt)));

  // Reset entries
  gtk_entry_set_text(GTK_ENTRY (kp), "");
  gtk_entry_set_text(GTK_ENTRY (ki), "");
  gtk_entry_set_text(GTK_ENTRY (kd), "");
  gtk_entry_set_text(GTK_ENTRY (dt), "");

  mpp.kp = kp_value;
  mpp.ki = ki_value;
  mpp.kd = kd_value;
  mpp.dt = dt_value;
  mpp.idt = 1/dt_value;

  PrintPacket(&mpp.h, buf);
  printf("%s\n",buf);
  
  // Sending the packet
  sendPacket(_fd, _ph, (PacketHeader*)&mpp, _send_sem);

}

/*
void destroy(GtkWidget* widget, gpointer data) {
  active=1;
  gtk_main_quit();
}
*/

void initGUI(int fd, uint8_t* end_flag, uint8_t* send_sem, PacketHandler* ph, MotorStatusPacket* motor_status) {
  pthread_t threads[1];
  
  _motor_status = motor_status;
  _ph = ph;
  _end_flag = end_flag;
  _send_sem = send_sem;
  _fd = fd;
  
  /* Declare widgets */
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  GtkWidget *label_et;
  GtkWidget *label_ds;
  GtkWidget *label_ms;

  GtkWidget *label_pid;
  GtkWidget *label_kp;
  GtkWidget *label_ki;
  GtkWidget *label_kd;
  GtkWidget *label_dt;

  // Init gtk
  gtk_init (NULL, NULL);

  // Creating new window with settings
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "HERMES");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_container_set_border_width (GTK_CONTAINER (window), 40);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE); /* window is NOT resizable */

  // Adding a grid
  grid = gtk_grid_new ();
  gtk_grid_set_row_spacing (GTK_GRID (grid), 5);
  gtk_container_add (GTK_CONTAINER (window), grid);


  // Adding labels
  label_et = gtk_label_new("Encoder Ticks:  ");
  label_ds = gtk_label_new("Desired Speed:  ");
  label_ms = gtk_label_new("Measured Speed:  ");

  et = gtk_label_new("0");
  ds = gtk_label_new("0");
  ms = gtk_label_new("0");

  label_pid = gtk_label_new("** Pid settings **");
  label_kp = gtk_label_new("Kp: ");
  label_ki = gtk_label_new("Ki: ");
  label_kd = gtk_label_new("Kd: ");
  label_dt = gtk_label_new("dt: ");


  gtk_grid_attach (GTK_GRID (grid), label_et, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), et, 1, 0, 1, 1);

  gtk_grid_attach (GTK_GRID (grid), label_ds, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), ds, 1, 1, 2, 1);

  gtk_grid_attach (GTK_GRID (grid), label_ms, 0, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), ms, 1, 2, 1, 1);

  // Adding packet parameters

  gtk_grid_attach (GTK_GRID (grid), label_pid, 0, 3, 2, 1);

  gtk_grid_attach (GTK_GRID (grid), label_kp, 0, 4, 1, 1);
  kp = gtk_entry_new();
  gtk_entry_set_placeholder_text (GTK_ENTRY (kp), "Kp");
  gtk_grid_attach (GTK_GRID (grid), kp, 1, 4, 1, 1);

  gtk_grid_attach (GTK_GRID (grid), label_ki, 0, 5, 1, 1);
  ki = gtk_entry_new();
  gtk_entry_set_placeholder_text (GTK_ENTRY (ki), "Ki");
  gtk_grid_attach (GTK_GRID (grid), ki, 1, 5, 1, 1);

  gtk_grid_attach (GTK_GRID (grid), label_kd, 0, 6, 1, 1);
  kd = gtk_entry_new();
  gtk_entry_set_placeholder_text (GTK_ENTRY (kd), "Kd");
  gtk_grid_attach (GTK_GRID (grid), kd, 1, 6, 1, 1);

  gtk_grid_attach (GTK_GRID (grid), label_dt, 0, 7, 1, 1);
  dt = gtk_entry_new();
  gtk_entry_set_placeholder_text (GTK_ENTRY (dt), "dt");
  gtk_grid_attach (GTK_GRID (grid), dt, 1, 7, 1, 1);

  // Send packet button
  button = gtk_button_new_with_label ("Send");
  g_signal_connect (button, "clicked", G_CALLBACK (send_packet), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 8, 2, 1);

  /*
  // Quit button
  button = gtk_button_new_with_label ("Quit");
  g_signal_connect (button, "clicked", G_CALLBACK (destroy), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 9, 2, 1);
  */
  
  // Showing all the widgets
  gtk_widget_show_all (window);

  // Creating thread to manage the received packets
  pthread_create(&threads[0], NULL, routine, NULL);

  // Controlling the gtk main
  gtk_main();

  // Waiting for the pthread to exit
  pthread_join(threads[0], NULL);

}

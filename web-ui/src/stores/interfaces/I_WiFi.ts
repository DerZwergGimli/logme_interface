export interface I_WiFi {
  config?: I_WiFi_Config;
  aps?: Array<I_WiFi_AP>;
}
export interface I_WiFi_Config {
  ssid: string;
  ip: string;
  netmask: string;
  gateway: string;
  urc: number;
}

export interface I_WiFi_AP {
  ssid: string;
  chan: number;
  rssi: number;
  auth: number;
}

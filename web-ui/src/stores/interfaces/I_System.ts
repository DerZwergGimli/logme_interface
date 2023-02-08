export interface I_System {
  info: I_Info;
}

export interface I_Info {
  target: string;
  cores: number;
  major_rev: number;
  minor_rev: number;
  flash_size_MB: number;
  total_heap: number;
  free_heap: number;
  uptime_ms: number;
  time: string;
  bin_version: string;
}

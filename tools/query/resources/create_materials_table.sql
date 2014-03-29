create table `materials` (
  `id` integer not null primary key autoincrement,
  `parent_model_id` integer not null,
  `index` integer not null,
  `name_ja` text not null,
  `name_en` text not null,
  `edge_size` real not null,
  `is_casting_shadow_enabled` integer not null,
  `is_casting_shadow_map_enabled` integer not null,
  `is_culling_disabled` integer not null,
  `is_edge_enabled` integer not null,
  `is_shadow_map_enabled` integer not null,
  `is_shared_toon_texture_used` integer not null,
  `is_vertex_color_enabled` integer not null,
  `main_texture_path` text null,
  `sphere_texture_path` text null,
  `toon_texture_path` text null,
  `index_range_count` integer not null,
  `user_data` text not null,
  foreign key (`parent_model_id`) references models(`id`)
 );